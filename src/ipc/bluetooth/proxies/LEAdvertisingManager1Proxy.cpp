#include "LEAdvertisingManager1Proxy.hpp"

namespace IPC::Bluetooth::Proxies {
LEAdvertisingManager1Proxy::LEAdvertisingManager1Proxy(
    sdbus::IConnection& connection, sdbus::ServiceName destination, sdbus::ObjectPath objectPath)
    : Base_t(connection, std::move(destination), std::move(objectPath))
{
    registerProxy();
}

LEAdvertisingManager1Proxy::~LEAdvertisingManager1Proxy()
{
    unregisterProxy();
}

sdbus::ObjectPath LEAdvertisingManager1Proxy::getRegisteredObjectPath() const
{
    return getProxy().getObjectPath();
}

void LEAdvertisingManager1Proxy::initialize()
{
}

void LEAdvertisingManager1Proxy::deinitialize()
{
}
} // namespace IPC::Bluetooth::Proxies
