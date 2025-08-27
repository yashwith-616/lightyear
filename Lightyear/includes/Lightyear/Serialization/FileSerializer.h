#pragma once

#include "Lightyear/Serialization/StreamReader.h"
#include "Lightyear/Serialization/StreamWriter.h"

// -------------------------------------------------
// File Write Stream
// -------------------------------------------------
namespace ly {

/*
 TODO:
    1. Need to add file version header
    2. Chunk based file stream save
    3. Chunk migration must be present in the components class
 */
class LIGHTYEAR_API FileStreamWriter : public StreamWriter {
public:
    FileStreamWriter(const std::filesystem::path& filePath);
    FileStreamWriter(const FileStreamWriter&) = delete;
    FileStreamWriter& operator=(const FileStreamWriter&) = delete;
    ~FileStreamWriter() override;

    [[nodiscard]] bool IsStreamGood() const override;
    uint64_t GetStreamPosition() override;
    void SetStreamPosition(uint64_t position) override;
    bool WriteData(const char* data, uint64_t size) override;

private:
    std::filesystem::path m_FilePath;
    std::ofstream m_Stream;
};

} // namespace ly


// -------------------------------------------------
// File Read Stream
// -------------------------------------------------
namespace ly {

class LIGHTYEAR_API FileStreamReader : public StreamReader {
public:
    FileStreamReader(const std::filesystem::path& filePath);
    FileStreamReader(const FileStreamReader&) = delete;
    FileStreamReader& operator=(const FileStreamReader&) = delete;
    ~FileStreamReader() override;

    [[nodiscard]] bool IsStreamGood() const override;
    uint64_t GetStreamPosition() override;
    void SetStreamPosition(uint64_t position) override;
    bool ReadData(char* destination, uint64_t size) override;

private:
    std::filesystem::path m_FilePath;
    std::ifstream m_Stream;
};

} // namespace ly