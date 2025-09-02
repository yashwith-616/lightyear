#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Serialization/SerializableContract.h"

namespace ly {

class LIGHTYEAR_API StreamWriter {
public:
    virtual ~StreamWriter() = default;

    operator bool() const { return IsStreamGood(); }

    [[nodiscard]] virtual bool IsStreamGood() const         = 0;
    [[nodiscard]] virtual uint64_t GetStreamPosition()      = 0;
    virtual void SetStreamPosition(uint64_t position)       = 0;
    virtual bool WriteData(const char* data, uint64_t size) = 0;

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
        if constexpr (HasSerializeContract<T>) {
            // Contract exists → verify required members
            LY_CORE_ASSERT(!HasVersion<T>,
                           "Serializable class missing 'version'");
            LY_CORE_ASSERT(!HasSerialize<T>,
                           "Serializable class missing static 'Serialize(StreamWriter&, const T&)'");

            T::Serialize(this, obj);
        } else {
            // Class did not inherit the contract → ensure it does not define serialization members
            // Ensures that the contract is not forgotten by chance.
            constexpr bool has_any_serialization = HasSerialize<T> || HasDeserialize<T>;
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

}  // namespace ly