#pragma once

#include "generated_interfaces/org.bluez.LEAdvertisingManager1_proxy.hpp"
#include "ipc/bluetooth/interfaces/IDBusObjectExistable.hpp"

#include <sdbus-c++/sdbus-c++.h>

namespace IPC::Bluetooth::Proxies {
class LEAdvertisingManager1Proxy final : public sdbus::ProxyInterfaces<org::bluez::LEAdvertisingManager1_proxy>,
                                         public IPC::Bluetooth::Interfaces::IDBusObjectExistable {

public:
    using Base_t = sdbus::ProxyInterfaces<org::bluez::LEAdvertisingManager1_proxy>;

    explicit LEAdvertisingManager1Proxy(
        sdbus::IConnection& connection, sdbus::ServiceName destination, sdbus::ObjectPath objectPath);

    virtual ~LEAdvertisingManager1Proxy();

    void initialize() override;

    void deinitialize() override;

    sdbus::ObjectPath getRegisteredObjectPath() const override;
};
} // namespace IPC::Bluetooth::Proxies