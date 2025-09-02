#pragma once

#include "Lightyear/LightyearCore.h"
#include "SerializableContract.h"

namespace ly {

class LIGHTYEAR_API StreamReader {
public:
    virtual ~StreamReader() = default;

    [[nodiscard]] virtual bool IsStreamGood() const       = 0;
    [[nodiscard]] virtual uint64_t GetStreamPosition()    = 0;
    virtual void SetStreamPosition(uint64_t position)     = 0;
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
        if constexpr (HasSerializeContract<T>) {
            // Contract exists → verify required members
            LY_CORE_ASSERT(!HasVersion<T>,
                           "Serializable class missing 'version'");
            LY_CORE_ASSERT(!HasDeserialize<T>,
                           "Serializable class missing static 'Deserialize(StreamReader&, T&)'");

            T::Deserialize(this, obj);
        } else {
            // Class did not inherit the contract → ensure it does not define serialization members
            // Ensures that the contract is not forgotten by chance.
            constexpr bool has_any_serialization = HasSerialize<T> || HasDeserialize<T>;
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
