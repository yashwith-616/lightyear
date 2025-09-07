#pragma once

#include "BinarySerialization.h"

namespace ly {

#pragma pack(push, 1)
/// \brief Represents the fixed-size header of a LYFS file.
///
/// This struct is written at the beginning of every serialized file in the LYFS format. It allows file readers to:
/// 1. Identify that the file is of LYBS type (using the magic number).
/// 2. Determine the version of the file format (using version).
///
/// \note The pragma pack(1) is required to prevent the compiler from
/// inserting padding between 'magic' and 'version', ensuring the struct
/// has an exact size of 8 bytes for consistent file layout.
struct FileHeader {
    char magic[4]    = { 'L', 'Y', 'B', 'F' };
    uint32_t version = 1;
};
#pragma pack(pop)

/// \brief Writes binary data to file
class LIGHTYEAR_API BinaryFileSerializer final : public BinarySerializer {
public:
    /// \brief Constructs a writer for the given filepath
    explicit BinaryFileSerializer(std::filesystem::path filePath);

    BinaryFileSerializer(const BinaryFileSerializer&)            = delete;
    BinaryFileSerializer& operator=(const BinaryFileSerializer&) = delete;
    BinaryFileSerializer(BinaryFileSerializer&&)                 = delete;
    BinaryFileSerializer& operator=(BinaryFileSerializer&&)      = delete;
    ~BinaryFileSerializer() override;

    [[nodiscard]] bool IsStreamGood() const override;
    uint64_t GetStreamPosition() override;
    void SetStreamPosition(uint64_t position) override;
    bool WriteData(const char* data, uint64_t size) override;

private:
    std::filesystem::path m_FilePath;
    std::ofstream m_Stream;

    void WriteHeader();
};

/// \brief Reads binary data from file
class LIGHTYEAR_API BinaryFileDeserializer final : public BinaryDeserializer {
public:
    /// \brief Constructs a reader for the given function
    explicit BinaryFileDeserializer(const std::filesystem::path& filePath);

    BinaryFileDeserializer(const BinaryFileDeserializer&)            = delete;
    BinaryFileDeserializer& operator=(const BinaryFileDeserializer&) = delete;
    BinaryFileDeserializer(BinaryFileDeserializer&&)                 = delete;
    BinaryFileDeserializer& operator=(BinaryFileDeserializer&&)      = delete;
    ~BinaryFileDeserializer() override;

    [[nodiscard]] bool IsStreamGood() const override;
    uint64_t GetStreamPosition() override;
    void SetStreamPosition(uint64_t position) override;
    bool ReadData(char* destination, size_t size) override;

private:
    std::filesystem::path m_FilePath;
    std::ifstream m_Stream;
    int m_FileVersion{};

    bool validateHeader();
};

}  // namespace ly