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

class UuidGenerator {
public:
    UuidGenerator() : m_engine(m_randomDevice()) {}

    uint64_t generate() { return m_distribution(m_engine); }

private:
    std::random_device m_randomDevice;
    std::mt19937_64 m_engine;
    std::uniform_int_distribution<uint64_t> m_distribution;
};

class Uuid {
public:
    Uuid();
    explicit Uuid(uint64_t uuid);

    bool operator==(Uuid const& other) const { return m_uuid == other.m_uuid; }
    bool operator!=(Uuid const& other) const { return !(*this == other); }

    [[nodiscard]] uint64_t get() const { return m_uuid; }

private:
    uint64_t m_uuid;
};

static UuidGenerator& getUuidGenerator() {
    static UuidGenerator generator;
    return generator;
}

}  // namespace ly
