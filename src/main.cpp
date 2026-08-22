#include "data_dump/BytesSerializer.hpp"
#include "data_dump/DataDumper.hpp"
#include "device_reader/DeviceDataReader.hpp"

#include <unistd.h>

#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    std::string dumpPath, gpsDevicePath;
    int32_t opt;
    // Retrieve the options:
    while ((opt = getopt(argc, argv, "o:g:")) != -1) {
        switch (opt) {
        case 'g': {
            gpsDevicePath = optarg;
            break;
        }
        case 'o': {
            dumpPath = optarg;
            break;
        }
        default:
            break;
        }
    }

    if (dumpPath.empty() || gpsDevicePath.empty()) {
        std::cout << "Error. Empty path: dumpPath: " << dumpPath << ", gpsDevicePath" << gpsDevicePath << std::endl;
        return -1;
    }

    constexpr uint32_t dumpInterval = 1000; // ms
    std::shared_ptr<DataDump::DataDumper> mDataDumper = std::make_shared<DataDump::DataDumper>(
        dumpInterval, std::make_shared<DataDump::BytesSerializer>(dumpPath, DataDump::eWriteFileMode::APPEND));
    mDataDumper->initialize();

    std::unique_ptr<Device::DeviceDataReader> mGpsDeviceReader = std::make_unique<Device::DeviceDataReader>(gpsDevicePath);
    mGpsDeviceReader->subscribeToReadDataNotification(
        [dumperWeak = std::weak_ptr<DataDump::DataDumper>(mDataDumper)](const auto& pendingBytes) {
            if (const auto& dumperSp = dumperWeak.lock()) {
                dumperSp->addPendingForDumpBytes(pendingBytes);
            }
        });

    if (mGpsDeviceReader->tryToStartWatcher()) {
        std::cout << "Successfully started device reader" << std::endl;
    } else {
        std::cout << "Couldn't start device reader" << std::endl;
    }

    while (mDataDumper->isDumpingActive() || mGpsDeviceReader->isListenerActive()) { }
    return 0;
}