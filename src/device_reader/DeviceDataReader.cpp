#include "DeviceDataReader.hpp"

#include <fcntl.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <cerrno>
#include <iostream>

namespace DeviceReader {

struct DeviceDataReader::Impl {

    explicit Impl(const std::string& pathToDevice, DeviceDataReader* owner)
        : mPathToDevice(pathToDevice)
        , mOwner(owner)
    {
    }

    ~Impl()
    {
        stopListenerThread();
        closeWatch();
    }

    bool initWatch()
    {
        std::cout << "Start to watch " << mPathToDevice << std::endl;
        if (mFileDescriptor != -1 || mEPollFileDescriptor != -1) {
            std::cout << "Watch already initialized! Skipping." << std::endl;
            return false;
        }

        mFileDescriptor = open(mPathToDevice.c_str(), O_RDONLY | O_NONBLOCK); // for DBG cases O_RDWR to keep pipe fd opened
        if (-1 == mFileDescriptor) {
            int32_t errsv = errno;
            std::cout << "Failed to open file, error: " << errsv << std::endl;
            return false;
        }

        mEPollFileDescriptor = epoll_create1(0);
        if (-1 == mEPollFileDescriptor) {
            int32_t errsv = errno;
            std::cout << "Failed to create epoll, error: " << errsv << std::endl;
            return false;
        }

        epoll_event event;
        event.events = EPOLLIN;
        event.data.fd = mFileDescriptor;

        int32_t opResult = epoll_ctl(mEPollFileDescriptor, EPOLL_CTL_ADD, mFileDescriptor, &event);
        if (-1 == opResult) {
            int32_t errsv = errno;
            std::cout << "Failed to add epoll controller, error: " << errsv << std::endl;
            closeWatch();
            return false;
        }

        return true;
    }

    void closeWatch()
    {
        if (mEPollFileDescriptor != -1) {
            epoll_ctl(mEPollFileDescriptor, EPOLL_CTL_DEL, mFileDescriptor, nullptr);
        }
        if (mFileDescriptor != -1) {
            close(mFileDescriptor);
        }
        mEPollFileDescriptor = -1;
        mFileDescriptor = -1;
    }

    bool isListenerThreadActive() const
    {
        return mIsListenerThreadActive;
    }

    void startListenerThread()
    {
        if (mIsListenerThreadActive.load()) {
            std::cout << "Thread already started, skipping." << std::endl;
            return;
        }
        mIsListenerThreadActive.store(true);
        mListenerThread = std::thread(std::bind(&DeviceDataReader::Impl::poll, this));
    }

    void stopListenerThread()
    {
        mIsListenerThreadActive.store(false);
        if (mListenerThread.joinable()) {
            mListenerThread.join();
        }
    }

    void terminateOnError(uint32_t errorCode)
    {
        std::string errorStr;
        if (errorCode & EPOLLERR) {
            errorStr += "EPOLLERR ";
        }
        if (errorCode & EPOLLHUP) {
            errorStr += "EPOLLHUP ";
        }
        std::cout << "Terminate on error! Error code: " << errorStr << std::endl;
        mIsListenerThreadActive.store(false);
        closeWatch();
    }

    void poll()
    {
        while (mIsListenerThreadActive) {
            epoll_event events[1];
            const int32_t eventsCount = epoll_wait(mEPollFileDescriptor, events, 1, 2000);
            if (eventsCount <= 0 || mFileDescriptor != events[0].data.fd) {
                continue;
            }

            if (events[0].events & EPOLLIN) {

                std::vector<uint8_t> collectedBytes;
                ssize_t totalBytesRead = 0;
                char buffer[4096];
                const int32_t changedFd = events[0].data.fd;
                ssize_t length = 0;
                do {
                    length = read(changedFd, buffer, sizeof(buffer));
                    if (length > 0) {
                        const size_t currentSize = collectedBytes.size();
                        collectedBytes.reserve(currentSize + length);
                        for (int32_t i = 0; i < length; ++i) {
                            collectedBytes.push_back(buffer[i]);
                        }
                        totalBytesRead += length;
                    }
                } while (length >= static_cast<ssize_t>(sizeof(buffer)));

                if (mOwner && totalBytesRead > 0) {
                    mOwner->notifyReadData(collectedBytes);
                }
            } else if (events[0].events & (EPOLLERR | EPOLLHUP)) {
                terminateOnError(events[0].events);
                return;
            }
        }
    }

private:
    std::string mPathToDevice;

    DeviceDataReader* mOwner;

    std::atomic_int32_t mFileDescriptor{-1};
    std::atomic_int32_t mEPollFileDescriptor{-1};

    std::thread mListenerThread;
    std::atomic_bool mIsListenerThreadActive{false};
};

DeviceDataReader::DeviceDataReader(const std::string& pathToDevice)
    : mImpl(std::make_unique<Impl>(pathToDevice, this))
{
}

DeviceDataReader::~DeviceDataReader()
{
    if (mImpl) {
        stopListenerThread();
    }
}

bool DeviceDataReader::tryToStartWatcher()
{
    if (mImpl->initWatch()) {
        mImpl->startListenerThread();
        return true;
    }

    return false;
}

bool DeviceDataReader::isListenerActive() const
{
    return mImpl->isListenerThreadActive();
}

void DeviceDataReader::stopListenerThread()
{
    mImpl->stopListenerThread();
}

void DeviceDataReader::subscribeToReadDataNotification(std::function<void(const std::vector<uint8_t>&)> callback)
{
    std::lock_guard<std::mutex> lock(mSubscribersMutex);
    mReadDataCallbacks.emplace_back(std::move(callback));
}

void DeviceDataReader::notifyReadData(const std::vector<uint8_t>& data)
{
    std::cout << "Notify read data with " << data.size() << " bytes" << std::endl;
    decltype(mReadDataCallbacks) callbacks;
    {
        std::lock_guard<std::mutex> lock(mSubscribersMutex);
        callbacks = mReadDataCallbacks;
    }

    for (const auto& readDataCallback : callbacks) {
        std::invoke(readDataCallback, data);
    }
}
} // namespace DeviceReader
