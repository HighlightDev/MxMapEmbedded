#include "BytesSerializer.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <iterator>

namespace fs = std::filesystem;

namespace DataDump {

BytesSerializer::BytesSerializer(const std::string& pathToDumpFile, const eWriteFileMode mode)
    : SerializerBase(pathToDumpFile, mode)
{
    mOpenMode = std::ios_base::binary;
    mOpenMode |= eWriteFileMode::APPEND == mode ? std::ios_base::app : static_cast<std::ios_base::openmode>(0);
}

void BytesSerializer::serialize(const std::vector<uint8_t>& bytesToSerialize)
{
    if (validatePath()) {
        std::ofstream os(mPathToDumpFile + "/dbg_output", mOpenMode);
        if (os.is_open()) {
            os.write(
                reinterpret_cast<const char*>(bytesToSerialize.data()), static_cast<std::streamsize>(bytesToSerialize.size()));
        } else {
            std::cout << "BytesSerializer::serialize: failed to open " << mPathToDumpFile + "/dbg_output" << std::endl;
        }
    }
}

bool BytesSerializer::validatePath() const
{
    std::error_code ec;
    if (fs::exists(mPathToDumpFile, ec)) {
        return true;
    }
    const bool ok = fs::create_directory(mPathToDumpFile, ec);
    if (ec) {
        std::cout << "BytesSerializer::validatePath: " << ec.message() << std::endl;
        return false;
    }
    return ok;
}
} // namespace DataDump
