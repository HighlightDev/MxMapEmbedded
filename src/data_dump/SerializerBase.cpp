#include "SerializerBase.hpp"

namespace DataDump {
SerializerBase::SerializerBase(const std::string& pathToDumpFile, const eWriteFileMode mode)
    : mPathToDumpFile(pathToDumpFile)
    , mWriteFileMode(mode)
{
}

} // namespace DataDump
