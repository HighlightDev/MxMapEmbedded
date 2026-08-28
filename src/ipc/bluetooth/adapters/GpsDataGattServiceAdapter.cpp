#include "GpsDataGattServiceAdapter.hpp"

namespace IPC::Bluetooth::Adapters {
constexpr uint16_t c_autoAllocatedHandle = 0x0000;

GpsDataGattServiceAdapter::GpsDataGattServiceAdapter(sdbus::IConnection& connection, sdbus::ObjectPath objectPath)
    : Base_t(connection, std::move(objectPath))
    , mProvidedInterfaces({sdbus::InterfaceName(GattService1_adaptor::INTERFACE_NAME)})
    , mUUID("2b8340d1-22b6-4612-b6e8-8914b82614d3")
{
    registerAdaptor();
}

GpsDataGattServiceAdapter::~GpsDataGattServiceAdapter()
{
    unregisterAdaptor();
}

sdbus::ObjectPath GpsDataGattServiceAdapter::getRegisteredObjectPath() const
{
    return getObject().getObjectPath();
}

void GpsDataGattServiceAdapter::initialize()
{
}

void GpsDataGattServiceAdapter::deinitialize()
{
}

std::string GpsDataGattServiceAdapter::UUID()
{
    return mUUID;
}

bool GpsDataGattServiceAdapter::Primary()
{
    return true;
}

uint16_t GpsDataGattServiceAdapter::Handle()
{
    return c_autoAllocatedHandle;
}

void GpsDataGattServiceAdapter::Handle([[maybe_unused]] const uint16_t& value)
{
}
} // namespace IPC::Bluetooth::Adapters
