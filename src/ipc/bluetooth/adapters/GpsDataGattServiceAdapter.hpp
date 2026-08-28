#pragma once

#include "generated_interfaces/org.bluez.GattService1_adapter.hpp"
#include "ipc/bluetooth/interfaces/IDBusObjectExistable.hpp"

#include <sdbus-c++/sdbus-c++.h>

namespace IPC::Bluetooth::Adapters {
class GpsDataGattServiceAdapter final
    : public sdbus::AdaptorInterfaces<org::bluez::GattService1_adaptor, sdbus::ManagedObject_adaptor>,
      public IPC::Bluetooth::Interfaces::IDBusObjectExistable {
    using Base_t = sdbus::AdaptorInterfaces<org::bluez::GattService1_adaptor, sdbus::ManagedObject_adaptor>;

    const std::vector<sdbus::InterfaceName> mProvidedInterfaces;

    const std::string mUUID;

public:
    explicit GpsDataGattServiceAdapter(sdbus::IConnection& connection, sdbus::ObjectPath objectPath);

    ~GpsDataGattServiceAdapter();

    sdbus::ObjectPath getRegisteredObjectPath() const override;

    std::string UUID() override;

    bool Primary() override;

    uint16_t Handle() override;

    void Handle(const uint16_t& value) override;

    void initialize() override;

    void deinitialize() override;
};
} // namespace IPC::Bluetooth::Adapters
