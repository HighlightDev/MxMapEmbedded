#pragma once

#include <stdint.h>

#include <string>
#include <vector>

namespace DataDump {
enum class eWriteFileMode { APPEND, OVERWRITE };

class SerializerBase {

protected:
    const std::string mPathToDumpFile;

    const eWriteFileMode mWriteFileMode;

public:
    explicit SerializerBase(const std::string& pathToDumpFile, const eWriteFileMode mode);

    virtual void serialize(const std::vector<uint8_t>& bytesToSerialize) = 0;
};
} // namespace DataDump
