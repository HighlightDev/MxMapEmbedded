#pragma once

#include "sdbus-c++/sdbus-c++.h"

namespace IPC::Bluetooth::Proxies {

class ObjectManagerProxy final : public sdbus::ProxyInterfaces<sdbus::ObjectManager_proxy> {
public:
    explicit ObjectManagerProxy(sdbus::IConnection& connection, sdbus::ServiceName destination, sdbus::ObjectPath objectPath);

    virtual ~ObjectManagerProxy();

    void handleExistingObjects();

private:
    void onInterfacesAdded(
        const sdbus::ObjectPath& objectPath,
        const std::map<sdbus::InterfaceName, std::map<sdbus::PropertyName, sdbus::Variant>>& interfacesAndProperties) override;
    void onInterfacesRemoved(const sdbus::ObjectPath& objectPath, const std::vector<sdbus::InterfaceName>& interfaces) override;
};
} // namespace IPC::Bluetooth::Proxies
