#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Serialization/Concepts.h"

namespace ly {

/// \brief Base class for Binary Serializer
///
/// BinarySerializer provides an interface to write objects to a binary format. It is designed to be extended for
/// specific targets such as files, memory buffers or network streams.
class LIGHTYEAR_API BinarySerializer {
public:
    virtual ~BinarySerializer() = default;

    /// \brief Check if stream is ready for write
    /// \return true, if stream is ready
    [[nodiscard]] virtual bool IsStreamGood() const = 0;

    /// \brief Get current stream position in bytes
    /// \return Current position of the stream
    [[nodiscard]] virtual uint64_t GetStreamPosition() = 0;

    /// \brief Set current stream position in bytes
    /// \param position - The position of the stream
    virtual void SetStreamPosition(uint64_t position) = 0;

    /// \brief Write data to the stream
    /// \param data the pointer to the data to read
    /// \param size the Number of bytes to read
    /// \return true, if write is successful
    virtual bool WriteData(const char* data, uint64_t size) = 0;

    operator bool() const { return IsStreamGood(); }

    void WriteVersion(Version version) { WriteData(reinterpret_cast<const char*>(&version), sizeof(version)); }

    void WriteZero(uint64_t size) {
        constexpr char zero = 0;
        for (uint64_t i = 0; i < size; i++) {
            WriteData(&zero, 1);
        }
    }

    void WriteString(const std::string& str) {
        size_t size = str.size();
        WriteData(reinterpret_cast<const char*>(&size), sizeof(size));
        WriteData(str.data(), sizeof(char) * str.size());
    }

    template <typename T>
    void WriteRaw(const T& type) {
        bool success = WriteData(reinterpret_cast<const char*>(&type), sizeof(type));
        LY_CORE_ASSERT(success, "Write raw failed");
    }

    /// \brief Writes an object of type T from the StreamWriter
    /// \details
    /// This function enforces serialization contracts at compile-time using concepts and
    /// custom asserts.
    ///
    /// \tparam T Type of the object being serialized. Must satisfy the serialization concepts.
    template <typename T>
    void WriteObject(const T& obj) {
        if constexpr (internal::has_serialize_contract_v<T>) {
            LY_CORE_ASSERT(!internal::has_serialize_v<T>,
                           "Serializable class missing static 'Serialize(StreamWriter&, const T&)'");

            T::Serialize(this, obj);
        } else {
            // Class did not inherit the contract → ensure it does not define serialization members
            // Ensures that the contract is not forgotten by chance.
            constexpr bool has_any_serialization = internal::has_serialize_v<T> || internal::has_deserialize_v<T>;
            LY_CORE_ASSERT(has_any_serialization,
                           "Class defines serialization members but does not inherit SerializableContract");
        }
    }

    template <typename Key, typename Value>
    void WriteMap(const std::map<Key, Value>& map, bool writeSize = false) {
        if (writeSize) {
            WriteRaw<uint32_t>(map.size());
        }

        for (const auto& [key, value] : map) {
            if constexpr (std::is_trivial<Key>()) {
                WriteRaw<Key>(sizeof(Key));
            } else {
                WriteObject<Key>(key);
            }

            if constexpr (std::is_trivial<Value>()) {
                WriteRaw<Value>(sizeof(Value));
            } else {
                WriteObject<Value>(value);
            }
        }
    }

    template <typename Key, typename Value>
    void WriteMap(const std::unordered_map<Key, Value>& map, bool writeSize = false) {
        if (writeSize) {
            WriteRaw<uint32_t>(map.size());
        }

        for (const auto& [key, value] : map) {
            if constexpr (std::is_trivial<Key>()) {
                WriteRaw<Key>(sizeof(Key));
            } else {
                WriteObject<Key>(key);
            }

            if constexpr (std::is_trivial<Value>()) {
                WriteRaw<Value>(sizeof(Value));
            } else {
                WriteObject<Value>(value);
            }
        }
    }
};

/// \brief Base class for Binary Deserializer
///
/// BinaryDeserializer provides an interface to read objects to a binary format. It is designed to be extended for
/// specific targets such as files, memory buffers or network streams.
class LIGHTYEAR_API BinaryDeserializer {
public:
    virtual ~BinaryDeserializer() = default;

    /// \brief Is the stream ready for reading
    [[nodiscard]] virtual bool IsStreamGood() const = 0;

    /// \brief Gets the stream position in bytes
    /// \return Current position of the stream
    [[nodiscard]] virtual uint64_t GetStreamPosition() = 0;

    /// \brief Set stream position in bytes
    /// \param position The new stream position
    virtual void SetStreamPosition(uint64_t position) = 0;

    /// \brief Read data to the destination string
    /// \param destination Pointer to the data that is read
    /// \param size Number of bytes to read
    virtual bool ReadData(char* destination, size_t size) = 0;

    operator bool() const { return IsStreamGood(); }

    Version ReadVersion() {
        int16_t version{};
        ReadData(reinterpret_cast<char*>(&version), sizeof(version));
        return Version(version);
    }

    std::string ReadString() {
        auto strSize = ReadRaw<size_t>();

        std::string destinationStr(strSize, '\0');
        if (strSize > 0) {
            ReadData(reinterpret_cast<char*>(&destinationStr), strSize);
        }

        return destinationStr;
    }

    template <typename T>
    T ReadRaw() {
        T value{};
        const bool success = ReadData(reinterpret_cast<char*>(&value), sizeof(value));
        LY_CORE_ASSERT(success, "Read raw failed");
        return value;
    }

    /// \brief Reads an object of type T from the StreamReader
    /// \details
    /// This function enforces serialization contracts at compile-time using concepts and
    /// custom asserts.
    ///
    /// \tparam T Type of the object being deserialized. Must satisfy the serialization concepts.
    template <typename T>
    void ReadObject(T& obj) {
        if constexpr (internal::has_serialize_contract_v<T>) {
            // Contract exists → verify required members
            LY_CORE_ASSERT(!internal::has_deserialize_v<T>,
                           "Serializable class missing static 'Deserialize(StreamReader&, T&)'");

            T::Deserialize(this, obj);
        } else {
            // Class did not inherit the contract → ensure it does not define serialization members
            // Ensures that the contract is not forgotten by chance.
            constexpr bool has_any_serialization = internal::has_serialize_v<T> || internal::has_deserialize_v<T>;
            LY_CORE_ASSERT(has_any_serialization,
                           "Class defines serialization members but does not inherit SerializableContract");
        }
    }

    template <typename Key, typename Value>
    void ReadMap(std::map<Key, Value>& map, bool readSize = false) {
        if (readSize) {
            ReadRaw<uint32_t>(map.size());
        }

        for (const auto& [key, value] : map) {
            if constexpr (std::is_trivial<Key>()) {
                ReadRaw<Key>(sizeof(Key));
            } else {
                ReadObject<Key>(key);
            }

            if constexpr (std::is_trivial<Value>()) {
                ReadRaw<Value>(sizeof(Value));
            } else {
                ReadObject<Value>(value);
            }
        }
    }

    template <typename Key, typename Value>
    void ReadMap(std::unordered_map<Key, Value>& map, bool readSize = false) {
        if (readSize) {
            ReadRaw<uint32_t>(map.size());
        }

        for (const auto& [key, value] : map) {
            if constexpr (std::is_trivial<Key>()) {
                ReadRaw<Key>(sizeof(Key));
            } else {
                ReadObject<Key>(key);
            }

            if constexpr (std::is_trivial<Value>()) {
                ReadRaw<Value>(sizeof(Value));
            } else {
                ReadObject<Value>(value);
            }
        }
    }
};

}  // namespace ly
