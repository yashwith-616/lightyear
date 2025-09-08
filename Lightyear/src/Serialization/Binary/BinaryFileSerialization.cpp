#include "Lightyear/Serialization/Binary/BinaryFileSerialization.h"
#include <utility>
#include "Config.hpp"

namespace ly {

// -------------------------------------------------
// File Write Stream
// -------------------------------------------------
BinaryFileSerializer::BinaryFileSerializer(std::filesystem::path filePath)
    : m_FilePath(std::move(filePath)), m_Stream(m_FilePath, std::ios::out | std::ios::binary) {
    WriteHeader();
}

BinaryFileSerializer::~BinaryFileSerializer() {
    m_Stream.close();
}

uint64_t BinaryFileSerializer::GetStreamPosition() {
    return m_Stream.tellp();
}

bool BinaryFileSerializer::IsStreamGood() const {
    return m_Stream.good();
}

void BinaryFileSerializer::SetStreamPosition(uint64_t position) {
    m_Stream.seekp(ly::narrow_cast<std::streamoff>(position));
}

bool BinaryFileSerializer::WriteData(const char* data, uint64_t size) {
    m_Stream.write(data, ly::narrow_cast<std::streamsize>(size));
    return true;
}

void BinaryFileSerializer::WriteHeader() {
    FileHeader header{};
    m_Stream.write(reinterpret_cast<const char*>(&header), sizeof(header));
}

// -------------------------------------------------
// File Read Stream
// -------------------------------------------------
BinaryFileDeserializer::BinaryFileDeserializer(const std::filesystem::path& filePath)
    : m_FilePath(filePath), m_Stream(m_FilePath, std::ios::in | std::ios::binary) {
    validateHeader();
}

BinaryFileDeserializer::~BinaryFileDeserializer() {
    m_Stream.close();
}

uint64_t BinaryFileDeserializer::GetStreamPosition() {
    return m_Stream.tellg();
}

bool BinaryFileDeserializer::IsStreamGood() const {
    return m_Stream.good();
}

void BinaryFileDeserializer::SetStreamPosition(uint64_t position) {
    m_Stream.seekg(ly::narrow_cast<std::streamoff>(position));
}

bool BinaryFileDeserializer::ReadData(char* destination, uint64_t size) {
    m_Stream.read(destination, ly::narrow_cast<std::streamsize>(size));
    return true;
}

bool BinaryFileDeserializer::validateHeader() {
    FileHeader header{};
    m_Stream.read(reinterpret_cast<char*>(&header), sizeof(header));

    const bool isValidHeader = std::memcmp(header.magic, kFileHeaderMagic.data(), kFileHeaderMagic.size()) == 0;
    if (!isValidHeader) {
        LY_CORE_LOG(LogType::Error,
                    "File header mismatch; required {}, received {}",
                    kFileHeaderMagic,
                    std::string_view(header.magic, sizeof(header.magic)));
        return false;
    }

    m_FileVersion = header.version;
    return true;
}

}  // namespace ly
