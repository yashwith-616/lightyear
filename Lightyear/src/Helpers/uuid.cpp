#include "Lightyear/Helpers/uuid.h"

namespace ly {

UUID::UUID() : m_UUID(GetUUIDGenerator().Generate()) {}

UUID::UUID(uint64_t UUID) : m_UUID(UUID) {}

}  // namespace ly

namespace std {

template <>
struct hash<ly::UUID> {
    std::size_t operator()(const ly::UUID& uuid) const { return hash<uint64_t>()(uuid.Get()); }
};

}  // namespace std