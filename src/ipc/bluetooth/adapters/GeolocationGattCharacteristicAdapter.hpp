#pragma once

#include "generated_interfaces/org.bluez.GattCharacteristic1_adapter.hpp"

#include <sdbus-c++/sdbus-c++.h>

namespace IPC::Bluetooth::Adapters {
class GeolocationGattCharacteristicAdapter final : public sdbus::AdaptorInterfaces<org::bluez::GattCharacteristic1_adaptor> {
    using Base_t = sdbus::AdaptorInterfaces<org::bluez::GattCharacteristic1_adaptor>;

public:
    explicit GeolocationGattCharacteristicAdapter(sdbus::IConnection& connection, sdbus::ObjectPath objectPath);

    ~GeolocationGattCharacteristicAdapter();

private:
    std::vector<uint8_t> ReadValue(const std::map<std::string, sdbus::Variant>& options) override;

    void WriteValue(const std::vector<uint8_t>& value, const std::map<std::string, sdbus::Variant>& options) override;

    void StartNotify() override;

    void StopNotify() override;

    void Confirm() override;

private:
    std::string UUID() override;

    sdbus::ObjectPath Service() override;

    std::vector<uint8_t> Value() override;

    bool Notifying() override;

    std::vector<std::string> Flags() override;
};
} // namespace IPC::Bluetooth::Adapters
