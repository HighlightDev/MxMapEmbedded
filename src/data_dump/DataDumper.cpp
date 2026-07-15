#include "DataDumper.hpp"

#include "SerializerBase.hpp"

#include <assert.h>

#include <iostream>

namespace DataDump {

DataDumper::DataDumper(const uint32_t _dumpInterval, std::shared_ptr<SerializerBase> _serializer)
    : mDumpInterval(_dumpInterval)
    , mSerializer(_serializer)
{
    assert(_serializer != nullptr);
}

DataDumper::~DataDumper()
{
    mDumpingActive.store(false, std::memory_order::release);
    if (mDumpThread.joinable()) {
        mDumpThread.join();
    }
}

void DataDumper::initialize()
{
    if (mDumpingActive.load(std::memory_order::acquire)) {
        std::cout << "DataDumper::initialize: already initialized and thread already started. Returning.";
        return;
    }

    const uint32_t dumpIntervalSec = mDumpInterval / 1000;
    const uint32_t preciseBytesToAllocate = 10 * 10 * dumpIntervalSec; // 10 bytes per 10hz per dump interval in seconds
    const uint32_t bytesToAllocate
        = preciseBytesToAllocate + static_cast<uint32_t>(preciseBytesToAllocate * 0.5); // and extra buffer 50%
    mPendingForDumpBytes.reserve(bytesToAllocate);

    std::cout << "DataDumper::initialize: starting dump thread." << std::endl;
    mDumpingActive.store(true, std::memory_order_release);
    mDumpThread = std::thread([this]() { processBytes(); });
}

void DataDumper::addPendingForDumpBytes(const std::vector<uint8_t>& pendingBytes)
{
    if (pendingBytes.size() <= 0) {
        return;
    }
    std::lock_guard<std::mutex> lock(mDumpMutex);
    mPendingForDumpBytes.insert(mPendingForDumpBytes.end(), pendingBytes.cbegin(), pendingBytes.cend());
}

void DataDumper::processBytes()
{
    while (mDumpingActive.load(std::memory_order::acquire)) {
        std::vector<uint8_t> pendingData;
        {
            std::lock_guard<std::mutex> lock(mDumpMutex);
            pendingData = std::move(mPendingForDumpBytes);
            mPendingForDumpBytes.clear();
        }
        if (pendingData.size() > 0) {
            mSerializer->serialize(pendingData);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(mDumpInterval));
    }
}

void DataDumper::stopDumping()
{
    mDumpingActive.store(false, std::memory_order::release);
    if (mDumpThread.joinable()) {
        mDumpThread.join();
    }
}

bool DataDumper::isDumpingActive() const
{
    return mDumpingActive.load(std::memory_order::acquire);
}
} // namespace DataDump
