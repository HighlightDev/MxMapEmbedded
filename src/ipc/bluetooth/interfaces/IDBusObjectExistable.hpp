#pragma once

#include <sdbus-c++/sdbus-c++.h>

namespace IPC::Bluetooth::Interfaces {
class IDBusObjectExistable {
public:
    virtual void initialize() = 0;

    virtual void deinitialize() = 0;

    virtual sdbus::ObjectPath getRegisteredObjectPath() const = 0;
};
} // namespace IPC::Bluetooth::Interfaces
