#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace DeviceReader {

class DeviceDataReader {
public:
    explicit DeviceDataReader(const std::string& pathToDevice);

    virtual ~DeviceDataReader();

    bool tryToStartWatcher();

    void stopListenerThread();

    void subscribeToReadDataNotification(std::function<void(const std::vector<uint8_t>&)> callback);

    bool isListenerActive() const;

private:
    void notifyReadData(const std::vector<uint8_t>&);

private:
    struct Impl;
    std::unique_ptr<Impl> mImpl;

    std::vector<std::function<void(const std::vector<uint8_t>&)>> mReadDataCallbacks;

    std::mutex mSubscribersMutex;
};
} // namespace DeviceReader
