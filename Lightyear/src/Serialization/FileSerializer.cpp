#include "Lightyear/Serialization/FileSerializer.h"

// -------------------------------------------------
// File Write Stream
// -------------------------------------------------
namespace ly {

FileStreamWriter::FileStreamWriter(const std::filesystem::path& filePath)
    : m_FilePath(filePath), m_Stream(m_FilePath, std::ios::out | std::ios::binary) {}

FileStreamWriter::~FileStreamWriter() {
    m_Stream.close();
}

uint64_t FileStreamWriter::GetStreamPosition() {
    return m_Stream.tellp();
}

bool FileStreamWriter::IsStreamGood() const {
    return m_Stream.good();
}

void FileStreamWriter::SetStreamPosition(uint64_t position) {
    m_Stream.seekp(ly::narrow_cast<std::streamoff>(position));
}

bool FileStreamWriter::WriteData(const char* data, uint64_t size) {
    m_Stream.write(data, ly::narrow_cast<std::streamsize>(size));
    return true;
}

}  // namespace ly

// -------------------------------------------------
// File Read Stream
// -------------------------------------------------
namespace ly {

FileStreamReader::FileStreamReader(const std::filesystem::path& filePath)
    : m_FilePath(filePath), m_Stream(m_FilePath, std::ios::in | std::ios::binary) {}

FileStreamReader::~FileStreamReader() {
    m_Stream.close();
}

uint64_t FileStreamReader::GetStreamPosition() {
    return m_Stream.tellg();
}

bool FileStreamReader::IsStreamGood() const {
    return m_Stream.good();
}

void FileStreamReader::SetStreamPosition(uint64_t position) {
    m_Stream.seekg(ly::narrow_cast<std::streamoff>(position));
}

bool FileStreamReader::ReadData(char* destination, uint64_t size) {
    m_Stream.read(destination, ly::narrow_cast<std::streamsize>(size));
    return true;
}

}  // namespace ly
