#pragma once

#include <random>
#include <xhash>

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
    uint64_t Get() const { return m_UUID; }

private:
    uint64_t m_UUID;
};

static inline UUIDGenerator& GetUUIDGenerator() {
    static UUIDGenerator generator;
    return generator;
}

}  // namespace ly