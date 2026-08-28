#pragma once

#include "generated_interfaces/org.bluez.LEAdvertisement1_adapter.hpp"
#include "ipc/bluetooth/interfaces/IDBusObjectExistable.hpp"

#include <sdbus-c++/sdbus-c++.h>

namespace IPC::Bluetooth::Adapters {
class LEAdvertisement1Adapter final : public sdbus::AdaptorInterfaces<::org::bluez::LEAdvertisement1_adaptor>,
                                      public IPC::Bluetooth::Interfaces::IDBusObjectExistable {
    using Base_t = sdbus::AdaptorInterfaces<::org::bluez::LEAdvertisement1_adaptor>;

    const std::string mAdvertisingType;

public:
    explicit LEAdvertisement1Adapter(sdbus::IConnection& connection, sdbus::ObjectPath objectPath);

    ~LEAdvertisement1Adapter();

    void Release() override;

    void initialize() override;

    void deinitialize() override;

    sdbus::ObjectPath getRegisteredObjectPath() const override;

    /*
        Determines the type of advertising packet requested.
        Possible values:
        "broadcast":
        "peripheral":
    */
    std::string Type() override;

    /*/
        List of UUIDs to include in the "Service UUID" field of the Advertising Data.
    */
    std::vector<std::string> ServiceUUIDs() override;

    /*
        Manufacturer Data fields to include in the Advertising Data. Keys are the Manufacturer ID to
        associate with the data.
    */
    std::map<uint16_t, sdbus::Variant> ManufacturerData() override;

    /*
        List of UUIDs to include in the "Service Solicitation" field of the Advertising Data.
    */
    // OPTIONAL std::vector<std::string> SolicitUUIDs() override;

    /*
        Service Data elements to include in the Advertising Data. The keys are the UUID to associate
        with the data.
    */
    // OPTIONAL std::map<std::string, sdbus::Variant> ServiceData() override;

    /*
         Key is the advertising type and value is the data as byte array.
    */
    // OPTIONAL std::map<uint8_t, sdbus::Variant> Data() override;

    /*
        List of UUIDs to include in the "Service UUID" field of the Scan Response Data.
    */
    // OPTIONAL  std::vector<std::string> ScanResponseServiceUUIDs() override;

    // OPTIONAL  std::map<uint16_t, sdbus::Variant> ScanResponseManufacturerData() override;

    // OPTIONAL  std::vector<std::string> ScanResponseSolicitUUIDs() override;

    // OPTIONAL  std::map<std::string, sdbus::Variant> ScanResponseServiceData() override;

    // OPTIONAL  std::map<uint8_t, sdbus::Variant> ScanResponseData() override;

    bool Discoverable() override;

    /*
        The discoverable timeout in seconds. A value of zero means that the timeout is disabled and
        it will stay in discoverable/limited mode forever.
    */
    uint16_t DiscoverableTimeout() override;

    // OPTIONAL std::vector<std::string> Includes() override;

    std::string LocalName() override;

    // OPTIONAL uint16_t Appearance() override;

    uint16_t Duration() override;

    uint16_t Timeout() override;

    // OPTIONAL std::string SecondaryChannel() override;

    uint32_t MinInterval() override;

    uint32_t MaxInterval() override;

    // OPTIONAL int16_t TxPower() override;

    // OPTIONAL uint8_t Instance() override;

    // OPTIONAL void Instance(const uint8_t& value) override;
};
} // namespace IPC::Bluetooth::Adapters
