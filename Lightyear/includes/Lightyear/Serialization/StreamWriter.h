#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly {

class LIGHTYEAR_API StreamWriter {
public:
    virtual ~StreamWriter() = default;

    operator bool() const { return IsStreamGood(); }

    [[nodiscard]] virtual bool IsStreamGood() const         = 0;
    [[nodiscard]] virtual uint64_t GetStreamPosition()      = 0;
    virtual void SetStreamPosition(uint64_t position)       = 0;
    virtual bool WriteData(const char* data, uint64_t size) = 0;

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

    template <typename T>
    void WriteObject(const T& obj) {
        T::Serialize(this, obj);
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