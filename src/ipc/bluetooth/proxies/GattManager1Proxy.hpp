#pragma once

#include "generated_interfaces/org.bluez.GattManager1_proxy.hpp"
#include "ipc/bluetooth/interfaces/IDBusObjectExistable.hpp"

#include <sdbus-c++/sdbus-c++.h>

namespace IPC::Bluetooth::Proxies {
class GattManager1Proxy final : public sdbus::ProxyInterfaces<org::bluez::GattManager1_proxy>,
                                public IPC::Bluetooth::Interfaces::IDBusObjectExistable {

public:
    using Base_t = sdbus::ProxyInterfaces<org::bluez::GattManager1_proxy>;

    explicit GattManager1Proxy(sdbus::IConnection& connection, sdbus::ServiceName destination, sdbus::ObjectPath objectPath);

    virtual ~GattManager1Proxy();

    void initialize() override;

    void deinitialize() override;

    sdbus::ObjectPath getRegisteredObjectPath() const override;
};
} // namespace IPC::Bluetooth::Proxies