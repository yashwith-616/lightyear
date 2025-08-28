#pragma once

#include "Lightyear/Serialization/StreamReader.h"
#include "Lightyear/Serialization/StreamWriter.h"

namespace ly {

#pragma pack(push, 1)
struct FileHeader {
    char magic[4]    = { 'L', 'Y', 'F', 'S' };
    uint32_t version = 1;
};
#pragma pack(pop)

/*
 TODO:
    1. Need to add file version header
    2. Chunk based file stream save
    3. Chunk migration must be present in the components class
 */
/**
 * The class has methods that allows you to write to a file in binary format
 */
class LIGHTYEAR_API FileStreamWriter : public StreamWriter {
public:
    explicit FileStreamWriter(const std::filesystem::path& filePath);
    FileStreamWriter(const FileStreamWriter&)            = delete;
    FileStreamWriter& operator=(const FileStreamWriter&) = delete;
    FileStreamWriter(FileStreamWriter&&)                 = delete;
    FileStreamWriter& operator=(FileStreamWriter&&)      = delete;
    ~FileStreamWriter() override;

    [[nodiscard]] bool IsStreamGood() const override;
    uint64_t GetStreamPosition() override;
    void SetStreamPosition(uint64_t position) override;
    bool WriteData(const char* data, uint64_t size) override;

private:
    std::filesystem::path m_FilePath;
    std::ofstream m_Stream;

    void WriteHeader();
};

/**
 * The class helps to read data from a file that had been binary serialized
 */
class LIGHTYEAR_API FileStreamReader : public StreamReader {
public:
    explicit FileStreamReader(const std::filesystem::path& filePath);
    FileStreamReader(const FileStreamReader&)            = delete;
    FileStreamReader& operator=(const FileStreamReader&) = delete;
    FileStreamReader(FileStreamReader&&)                 = delete;
    FileStreamReader& operator=(FileStreamReader&&)      = delete;
    ~FileStreamReader() override;

    [[nodiscard]] bool IsStreamGood() const override;
    uint64_t GetStreamPosition() override;
    void SetStreamPosition(uint64_t position) override;
    bool ReadData(char* destination, uint64_t size) override;

private:
    std::filesystem::path m_FilePath;
    std::ifstream m_Stream;
    int m_FileVersion{};

    bool validateHeader();
};

}  // namespace ly