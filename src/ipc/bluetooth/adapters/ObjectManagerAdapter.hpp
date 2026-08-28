#pragma once

#include "ipc/bluetooth/interfaces/IDBusObjectExistable.hpp"

#include <sdbus-c++/sdbus-c++.h>

namespace IPC::Bluetooth::Adapters {
class ObjectManagerAdapter final : public sdbus::AdaptorInterfaces<sdbus::ObjectManager_adaptor>,
                                   public IPC::Bluetooth::Interfaces::IDBusObjectExistable {

    using Base_t = sdbus::AdaptorInterfaces<sdbus::ObjectManager_adaptor>;

public:
    explicit ObjectManagerAdapter(sdbus::IConnection& connection, sdbus::ObjectPath objectPath);

    ~ObjectManagerAdapter();

    void initialize() override;

    void deinitialize() override;

    sdbus::ObjectPath getRegisteredObjectPath() const override;
};
} // namespace IPC::Bluetooth::Adapters
