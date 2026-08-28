#include "GeolocationGattCharacteristicAdapter.hpp"

namespace IPC::Bluetooth::Adapters {

GeolocationGattCharacteristicAdapter::GeolocationGattCharacteristicAdapter(
    sdbus::IConnection& connection, sdbus::ObjectPath objectPath)
    : Base_t(connection, std::move(objectPath))
{
    registerAdaptor();
}

GeolocationGattCharacteristicAdapter::~GeolocationGattCharacteristicAdapter()
{
    unregisterAdaptor();
}

std::vector<uint8_t> GeolocationGattCharacteristicAdapter::ReadValue([
    [maybe_unused]] const std::map<std::string, sdbus::Variant>& options)
{
    return {};
}

void GeolocationGattCharacteristicAdapter::WriteValue(
    [[maybe_unused]] const std::vector<uint8_t>& value, [[maybe_unused]] const std::map<std::string, sdbus::Variant>& options)
{
}

void GeolocationGattCharacteristicAdapter::StartNotify()
{
}

void GeolocationGattCharacteristicAdapter::StopNotify()
{
}

void GeolocationGattCharacteristicAdapter::Confirm()
{
}

std::string GeolocationGattCharacteristicAdapter::UUID()
{
    return "";
}

sdbus::ObjectPath GeolocationGattCharacteristicAdapter::Service()
{
    return {};
}

std::vector<uint8_t> GeolocationGattCharacteristicAdapter::Value()
{
    return {};
}

bool GeolocationGattCharacteristicAdapter::Notifying()
{
    return false;
}

std::vector<std::string> GeolocationGattCharacteristicAdapter::Flags()
{
    return {};
}
} // namespace IPC::Bluetooth::Adapters
