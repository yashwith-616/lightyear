#pragma once

#include "Lightyear/Common/Macros.h"

LY_DISABLE_WARNINGS_PUSH
#include <random>

#ifdef _MSC_VER
    #include <xhash>
#else
    #include <unordered_map>
#endif

LY_DISABLE_WARNINGS_POP

namespace ly {

class UUIDGenerator {
public:
    UUIDGenerator() : m_Engine(m_RandomDevice()) {}

    uint64_t Generate() { return m_Distribution(m_Engine); }

private:
    std::random_device m_RandomDevice;
    std::mt19937_64 m_Engine;
    std::uniform_int_distribution<uint64_t> m_Distribution;
};

class UUID {
public:
    UUID();
    explicit UUID(uint64_t UUID);

    bool operator==(const UUID& other) const { return m_UUID == other.m_UUID; }
    bool operator!=(const UUID& other) const { return !(*this == other); }

    [[nodiscard]] uint64_t Get() const { return m_UUID; }

private:
    uint64_t m_UUID;
};

static UUIDGenerator& GetUUIDGenerator() {
    static UUIDGenerator generator;
    return generator;
}

}  // namespace ly
