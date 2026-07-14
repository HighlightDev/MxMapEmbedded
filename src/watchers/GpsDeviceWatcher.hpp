#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace Watchers {

class GpsDeviceWatcher {
public:
    explicit GpsDeviceWatcher(const std::string& pathToDevice);

    virtual ~GpsDeviceWatcher();

    bool tryToStartWatcher();

    void stopListenerThread();

    void subscribeToReadDataNotification(std::function<void(const std::vector<uint8_t>&)> callback);

private:
    void notifyReadData(const std::vector<uint8_t>&);

private:
    struct Impl;
    std::unique_ptr<Impl> mImpl;

    std::vector<std::function<void(const std::vector<uint8_t>&)>> mReadDataCallbacks;

    std::mutex mSubscribersMutex;
};
} // namespace Watchers
