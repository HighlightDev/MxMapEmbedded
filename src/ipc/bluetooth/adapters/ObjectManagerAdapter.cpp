#include "ObjectManagerAdapter.hpp"

namespace IPC::Bluetooth::Adapters {
ObjectManagerAdapter::ObjectManagerAdapter(sdbus::IConnection& connection, sdbus::ObjectPath objectPath)
    : Base_t(connection, std::move(objectPath))
{
    registerAdaptor();
}

ObjectManagerAdapter::~ObjectManagerAdapter()
{
    unregisterAdaptor();
}

sdbus::ObjectPath ObjectManagerAdapter::getRegisteredObjectPath() const
{
    return getObject().getObjectPath();
}

void ObjectManagerAdapter::initialize()
{
}

void ObjectManagerAdapter::deinitialize()
{
}
} // namespace IPC::Bluetooth::Adapters
