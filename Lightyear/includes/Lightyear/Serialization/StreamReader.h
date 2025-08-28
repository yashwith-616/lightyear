#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly {

class LIGHTYEAR_API StreamReader {
public:
    virtual ~StreamReader() = default;

    [[nodiscard]] virtual bool IsStreamGood() const         = 0;
    [[nodiscard]] virtual uint64_t GetStreamPosition()      = 0;
    virtual void SetStreamPosition(uint64_t position)       = 0;
    virtual bool ReadData(char* destination, uint64_t size) = 0;

    operator bool() const { return IsStreamGood(); }

    void ReadString(const std::string& str) {
        size_t size = str.size();
        ReadData(reinterpret_cast<char*>(&size), sizeof(size));
    }

    template <typename T>
    void ReadRaw(const T& type) {
        bool success = ReadData(reinterpret_cast<char*>(&type), sizeof(type));
        LY_CORE_ASSERT(success, "Read raw failed");
    }

    template <typename T>
    void ReadObject(const T& obj) {
        T::Deserialize(this, obj);
    }

    template <typename Key, typename Value>
    void ReadMap(const std::map<Key, Value>& map, bool readSize = false) {
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
    void ReadMap(const std::unordered_map<Key, Value>& map, bool readSize = false) {
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