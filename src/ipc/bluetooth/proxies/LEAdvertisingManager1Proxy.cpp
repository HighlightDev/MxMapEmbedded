#include "LEAdvertisingManager1Proxy.hpp"

namespace IPC::Bluetooth::Proxies {
LEAdvertisingManager1Proxy::LEAdvertisingManager1Proxy(
    sdbus::IConnection& connection, sdbus::ServiceName destination, sdbus::ObjectPath objectPath)
    : Base_t(connection, destination, objectPath)
{
    registerProxy();
}

LEAdvertisingManager1Proxy::~LEAdvertisingManager1Proxy()
{
    unregisterProxy();
}
} // namespace IPC::Bluetooth::Proxies
