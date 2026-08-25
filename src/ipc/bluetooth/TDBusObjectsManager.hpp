#pragma once

#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>

namespace IPC::Bluetooth {

template<typename T, typename Tuple>
struct has_type;

template<typename T>
struct has_type<T, std::tuple<>> : std::false_type {
    static constexpr bool value = false;
};

template<typename T, typename U, typename... Ts>
struct has_type<T, std::tuple<U, Ts...>> : has_type<T, std::tuple<Ts...>> {
};

template<typename T, typename... Ts>
struct has_type<T, std::tuple<T, Ts...>> : std::true_type {
    static constexpr bool value = true;
};

template<typename... Args>
class TDBusObjectsManager {
    using DBusObjectsTuple_t = std::tuple<Args...>;
    std::tuple<std::shared_ptr<Args>...> mDBusObjectsTypes;

public:
    explicit TDBusObjectsManager()
        : mDBusObjectsTypes((static_cast<std::shared_ptr<Args>>(nullptr))...)
    {
    }

    template<typename DBusObjectType, typename... ConstructorArgs>
    std::enable_if_t<has_type<DBusObjectType, DBusObjectsTuple_t>::value, std::shared_ptr<DBusObjectType>>
    createDBusObject(ConstructorArgs&&... args)
    {
        std::get<std::shared_ptr<DBusObjectType>>(mDBusObjectsTypes)
            = std::make_shared<DBusObjectType>(std::forward<ConstructorArgs>(args)...);
        return std::get<std::shared_ptr<DBusObjectType>>(mDBusObjectsTypes);
    }

    template<typename DBusObjectType>
    std::enable_if_t<has_type<DBusObjectType, DBusObjectsTuple_t>::value, std::shared_ptr<DBusObjectType>> getDBusObject() const
    {
        return std::get<std::shared_ptr<DBusObjectType>>(mDBusObjectsTypes);
    }
};
} // namespace IPC::Bluetooth
