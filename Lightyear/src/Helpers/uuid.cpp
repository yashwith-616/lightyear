#include "Lightyear/Helpers/UUID.h"

#include <random>

namespace ly {

static std::random_device s_RandomDevice;
static std::mt19937_64 s_Engine(s_RandomDevice());
static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

uuid::uuid() : m_UUID(s_UniformDistribution(s_Engine)) {}

uuid::uuid(uint64_t uuid) : m_UUID(uuid) {}

}  // namespace ly

namespace std {

template <>
struct hash<ly::uuid> {
    std::size_t operator()(const ly::uuid& uuid) const {
        return hash<uint64_t>()(static_cast<uint64_t>(uuid));
    }
};

}  // namespace std