#include "GattManager1Proxy.hpp"

namespace IPC::Bluetooth::Proxies {
GattManager1Proxy::GattManager1Proxy(sdbus::IConnection& connection, sdbus::ServiceName destination, sdbus::ObjectPath objectPath)
    : Base_t(connection, std::move(destination), std::move(objectPath))
{
    registerProxy();
}

GattManager1Proxy::~GattManager1Proxy()
{
    unregisterProxy();
}

sdbus::ObjectPath GattManager1Proxy::getRegisteredObjectPath() const
{
    return getProxy().getObjectPath();
}

void GattManager1Proxy::initialize()
{
}

void GattManager1Proxy::deinitialize()
{
}
} // namespace IPC::Bluetooth::Proxies
