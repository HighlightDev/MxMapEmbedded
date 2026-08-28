#include "ObjectManagerProxy.hpp"

#include <iostream>

namespace IPC::Bluetooth::Proxies {

ObjectManagerProxy::ObjectManagerProxy(
    sdbus::IConnection& connection, sdbus::ServiceName destination, sdbus::ObjectPath objectPath)
    : sdbus::ProxyInterfaces<sdbus::ObjectManager_proxy>(connection, std::move(destination), std::move(objectPath))
{
    registerProxy();
}

ObjectManagerProxy::~ObjectManagerProxy()
{
    unregisterProxy();
}

sdbus::ObjectPath ObjectManagerProxy::getRegisteredObjectPath() const
{
    return getProxy().getObjectPath();
}

void ObjectManagerProxy::initialize()
{
    handleExistingObjects();
}

void ObjectManagerProxy::deinitialize()
{
}

void ObjectManagerProxy::handleExistingObjects()
{
    const auto managedObjects = GetManagedObjects();
    for (const auto& [objectPath, interfacesAndProperties] : managedObjects) {
        onInterfacesAdded(objectPath, interfacesAndProperties);
    }
}

void ObjectManagerProxy::onInterfacesAdded(
    const sdbus::ObjectPath& objectPath,
    const std::map<sdbus::InterfaceName, std::map<sdbus::PropertyName, sdbus::Variant>>& interfacesAndProperties)
{
    std::cout << "Interfaces added to object path: " << objectPath << std::endl;
    for (const auto& [interface, properties] : interfacesAndProperties) {
        std::cout << "Interface added: " << interface << std::endl;
        for (const auto& [property, value] : properties) {
            std::cout << "Property: " << property << ", Value: " << value.dumpToString() << std::endl;
        }
    }
}

void ObjectManagerProxy::onInterfacesRemoved(
    const sdbus::ObjectPath& objectPath, const std::vector<sdbus::InterfaceName>& interfaces)
{
    std::cout << "Interfaces removed from object path: " << objectPath << std::endl;
    for (const auto& interface : interfaces) {
        std::cout << "Interface removed: " << interface << std::endl;
    }
}
} // namespace IPC::Bluetooth::Proxies
