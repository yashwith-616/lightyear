#pragma once

#include "Lightyear/Common/Macros.h"

LY_DISABLE_WARNINGS_PUSH
#include <random>
#include <xhash>
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
    [[nodiscard]] uint64_t Get() const { return m_UUID; }

    // Add equality operator
    bool operator==(const UUID& other) const { return m_UUID == other.m_UUID; }

    // Optionally add != for convenience
    bool operator!=(const UUID& other) const { return !(*this == other); }

private:
    uint64_t m_UUID;
};

static UUIDGenerator& GetUUIDGenerator() {
    static UUIDGenerator generator;
    return generator;
}

}  // namespace ly