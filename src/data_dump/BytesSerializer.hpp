#pragma once

#include "SerializerBase.hpp"

#include <fstream>
#include <string>

namespace DataDump {
class BytesSerializer : public SerializerBase {

    std::ios_base::openmode mOpenMode;

public:
    explicit BytesSerializer(const std::string& pathToDumpFile, const eWriteFileMode mode);

    void serialize(const std::vector<uint8_t>& bytesToSerialize) override;

private:
    bool validatePath() const;
};
} // namespace DataDump
