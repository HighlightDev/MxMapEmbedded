#include "data_dump/BytesSerializer.hpp"
#include "data_dump/DataDumper.hpp"
#include "device_reader/DeviceDataReader.hpp"
#include "ipc/bluetooth/TDBusObjectsManager.hpp"
#include "ipc/bluetooth/adapters/LEAdvertisement1Adapter.hpp"
#include "ipc/bluetooth/proxies/LEAdvertisingManager1Proxy.hpp"
#include "ipc/bluetooth/proxies/ObjectManagerProxy.hpp"

#include <unistd.h>

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

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

    std::unique_ptr<DeviceReader::DeviceDataReader> mGpsDeviceReader
        = std::make_unique<DeviceReader::DeviceDataReader>(gpsDevicePath);
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

    // DBus BlueZ communication test
    {
        auto connectionPtr = sdbus::createSystemBusConnection();

        const auto mngrPtr = std::make_unique<IPC::Bluetooth::TDBusObjectsManager<
            IPC::Bluetooth::Proxies::LEAdvertisingManager1Proxy,
            IPC::Bluetooth::Proxies::ObjectManagerProxy,
            IPC::Bluetooth::Adapters::LEAdvertisement1Adapter>>();

        mngrPtr->createDBusObject<IPC::Bluetooth::Proxies::LEAdvertisingManager1Proxy>(
            *connectionPtr, sdbus::ServiceName{"org.bluez"}, sdbus::ObjectPath{"/org/bluez/hci0"});

        mngrPtr->createDBusObject<IPC::Bluetooth::Proxies::ObjectManagerProxy>(
            *connectionPtr, sdbus::ServiceName{"org.bluez"}, sdbus::ObjectPath{"/"});

        const auto advertisementObjectPath = sdbus::ObjectPath{"/org/mxmap/advertisement"};

        mngrPtr->createDBusObject<IPC::Bluetooth::Adapters::LEAdvertisement1Adapter>(
            *connectionPtr, sdbus::ObjectPath{advertisementObjectPath});

        connectionPtr->enterEventLoopAsync();

        auto objectMngrPtr = mngrPtr->getDBusObject<IPC::Bluetooth::Proxies::ObjectManagerProxy>();

        if (objectMngrPtr) {
            objectMngrPtr->handleExistingObjects();
        }

        auto advertisementMngrPtr = mngrPtr->getDBusObject<IPC::Bluetooth::Proxies::LEAdvertisingManager1Proxy>();
        try {
            if (advertisementMngrPtr) {
                // Async call: it does not hold the connection while pending, so the event loop
                // thread stays free to answer BlueZ's GetAll on our advertisement object.
                auto pendingRegistration = advertisementMngrPtr->RegisterAdvertisement(advertisementObjectPath, {});
                pendingRegistration.get();
                std::cout << "Advertisement registered at " << advertisementObjectPath << std::endl;
            }
            constexpr int32_t secondsTimeToWait = 300;
            std::cout << "Advetisement will be active for " << secondsTimeToWait << " seconds" << std::endl;
            std::chrono::time_point startTime = std::chrono::steady_clock::now();
            int i = 0;
            while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime)
                   <= std::chrono::milliseconds(secondsTimeToWait * 1000)) {
                std::cout << "Seconds passed: " << i++ << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }

            if (advertisementMngrPtr) {
                advertisementMngrPtr->UnregisterAdvertisement(advertisementObjectPath);
                std::cout << "Advertisement unregistered at " << advertisementObjectPath << std::endl;
            }
        } catch (const std::exception& ex) {
            std::cout << "Exception during advertisement handling! Error message: " << ex.what() << std::endl;
        }

        connectionPtr->leaveEventLoop();
    }

    while (mDataDumper->isDumpingActive() || mGpsDeviceReader->isListenerActive()) { }
    return 0;
}