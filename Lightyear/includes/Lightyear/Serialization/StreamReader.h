#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly {

class LIGHTYEAR_API StreamReader {
public:
    virtual ~StreamReader() = default;

    [[nodiscard]] virtual bool IsStreamGood() const       = 0;
    [[nodiscard]] virtual uint64_t GetStreamPosition()    = 0;
    virtual void SetStreamPosition(uint64_t position)     = 0;
    virtual bool ReadData(char* destination, size_t size) = 0;

    operator bool() const { return IsStreamGood(); }

    uint16_t ReadVersion() {
        uint16_t version{};
        ReadData(reinterpret_cast<char*>(&version), sizeof(version));
        return version;
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

    template <typename T>
    void ReadObject(T& obj) {
        T::Deserialize(this, obj);
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