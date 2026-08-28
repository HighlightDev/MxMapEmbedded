#include "LEAdvertisement1Adapter.hpp"

#include <cassert>
#include <iostream>

namespace IPC::Bluetooth::Adapters {

LEAdvertisement1Adapter::LEAdvertisement1Adapter(sdbus::IConnection& connection, sdbus::ObjectPath objectPath)
    : Base_t(connection, std::move(objectPath))
    , mAdvertisingType("peripheral")
{
    registerAdaptor();
}

LEAdvertisement1Adapter::~LEAdvertisement1Adapter()
{
    unregisterAdaptor();
}

sdbus::ObjectPath LEAdvertisement1Adapter::getRegisteredObjectPath() const
{
    return getObject().getObjectPath();
}

void LEAdvertisement1Adapter::initialize()
{
}

void LEAdvertisement1Adapter::deinitialize()
{
}

void LEAdvertisement1Adapter::Release()
{
    std::cout << "LEAdvertisement1Adapter::Release" << std::endl;
}

std::string LEAdvertisement1Adapter::Type()
{
    return mAdvertisingType;
}

std::vector<std::string> LEAdvertisement1Adapter::ServiceUUIDs()
{
    return {}; // leave it empty, later we can provide service UUID when ble connection will be established
}

std::map<uint16_t, sdbus::Variant> LEAdvertisement1Adapter::ManufacturerData()
{
    constexpr uint16_t c_companyIdentifier = 0xFFFF; // personal
    constexpr uint8_t c_availableBytesForCustomData = 24;

    const uint8_t rawData[] = {0x68, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x77, 0x6F, 0x72, 0x6C, 0x64}; // Hello world
    const std::vector<uint8_t> customData(std::begin(rawData), std::end(rawData));
    static_assert(sizeof(rawData) <= c_availableBytesForCustomData);

    std::map<uint16_t, sdbus::Variant> result;
    result[c_companyIdentifier] = sdbus::Variant(customData);
    return result;
}

// std::vector<std::string> LEAdvertisement1Adapter::SolicitUUIDs()
// {
//     return {}; // empty, as we don't require any data from service to be trasferred from central device
// }

// std::map<std::string, sdbus::Variant> LEAdvertisement1Adapter::ServiceData()
// {
//     return {}; // already using ManufacturerData, skip service data as it shares the same size limit in bytes
// }

// std::map<uint8_t, sdbus::Variant> LEAdvertisement1Adapter::Data()
// {
//     return {}; // already using ManufacturerData, skip data
// }

// std::vector<std::string> LEAdvertisement1Adapter::ScanResponseServiceUUIDs()
// {
//     return {}; // for now empty as we are working as broadcast beacon
// }

// std::map<uint16_t, sdbus::Variant> LEAdvertisement1Adapter::ScanResponseManufacturerData()
// {
//     return {}; // for now empty as we are working as broadcast beacon
// }

// std::vector<std::string> LEAdvertisement1Adapter::ScanResponseSolicitUUIDs()
// {
//     return {}; // for now empty as we are working as broadcast beacon
// }

// std::map<std::string, sdbus::Variant> LEAdvertisement1Adapter::ScanResponseServiceData()
// {
//     return {}; // for now empty as we are working as broadcast beacon
// }

// std::map<uint8_t, sdbus::Variant> LEAdvertisement1Adapter::ScanResponseData()
// {
//     return {}; // for now empty as we are working as broadcast beacon
// }

bool LEAdvertisement1Adapter::Discoverable()
{
    return true;
}

uint16_t LEAdvertisement1Adapter::DiscoverableTimeout()
{
    return 0; // for now as we are broadcast beacon - always send advertisment packets
}

// std::vector<std::string> LEAdvertisement1Adapter::Includes()
// {
//     return {};
// }

std::string LEAdvertisement1Adapter::LocalName()
{
    return "MxMap";
}

// uint16_t LEAdvertisement1Adapter::Appearance()
// {
//     return 0;
// }

uint16_t LEAdvertisement1Adapter::Duration()
{
    return 20;
}

uint16_t LEAdvertisement1Adapter::Timeout()
{
    return 0;
}

// std::string LEAdvertisement1Adapter::SecondaryChannel()
// {
//     return "1M";
// }

uint32_t LEAdvertisement1Adapter::MinInterval()
{
    return 100;
}

uint32_t LEAdvertisement1Adapter::MaxInterval()
{
    return 1000;
}

// int16_t LEAdvertisement1Adapter::TxPower()
// {
//     return 0;
// }

// uint8_t LEAdvertisement1Adapter::Instance()
// {
//     return 0;
// }

// void LEAdvertisement1Adapter::Instance([[maybe_unused]] const uint8_t& value)
// {
// }

} // namespace IPC::Bluetooth::Adapters
