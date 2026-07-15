#pragma once

#include <stdint.h>

#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace DataDump {

class SerializerBase;

class DataDumper {
private:
    const uint32_t mDumpInterval;

    const std::shared_ptr<SerializerBase> mSerializer;

    std::vector<uint8_t> mPendingForDumpBytes;

    std::thread mDumpThread;

    std::mutex mDumpMutex;

    std::atomic_bool mDumpingActive{false};

public:
    explicit DataDumper(const uint32_t _dumpInterval, std::shared_ptr<SerializerBase> _serializer);

    ~DataDumper();

    void initialize();

    void addPendingForDumpBytes(const std::vector<uint8_t>& pendingBytes);

    void stopDumping();

    bool isDumpingActive() const;

private:
    void processBytes();
};
} // namespace DataDump
