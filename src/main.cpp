#include "data_dump/BytesSerializer.hpp"
#include "data_dump/DataDumper.hpp"
#include "device_reader/DeviceDataReader.hpp"

#include <iostream>
#include <string>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    constexpr uint32_t dumpInterval = 1000; // ms
    const std::string dumpPathToFile = "/tmp/dbg_output";
    std::shared_ptr<DataDump::DataDumper> mDataDumper = std::make_shared<DataDump::DataDumper>(
        dumpInterval, std::make_shared<DataDump::BytesSerializer>(dumpPathToFile, DataDump::eWriteFileMode::APPEND));
    mDataDumper->initialize();

    std::unique_ptr<Device::DeviceDataReader> mGpsDeviceReader = std::make_unique<Device::DeviceDataReader>("/tmp/dbg_pipe");
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