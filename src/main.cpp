#include "watchers/GpsDeviceWatcher.hpp"

#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    std::unique_ptr<Watchers::GpsDeviceWatcher> mGpsDeviceWatcher = std::make_unique<Watchers::GpsDeviceWatcher>("/tmp/dbg_pipe");
    const bool isStared = mGpsDeviceWatcher->tryToStartWatcher();
    if (isStared) {
        std::cout << "Successfully started watcher" << std::endl;
    } else {
        std::cout << "Couldn't start watcher" << std::endl;
    }
    auto listener = [](const auto& bytes) { std::cout << "Bytes size: " << bytes.size() << std::endl; };

    mGpsDeviceWatcher->subscribeToReadDataNotification(listener);

    while (isStared) { }
    return 0;
}