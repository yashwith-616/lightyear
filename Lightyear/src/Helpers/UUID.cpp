#include "Lightyear/Helpers/UUID.h"

namespace ly
{

Uuid::Uuid() : m_uuid(getUuidGenerator().generate()) {}

Uuid::Uuid(uint64_t uuid) : m_uuid(uuid) {}

} // namespace ly

namespace std
{

template <>
struct hash<ly::Uuid>
{
    std::size_t operator()(ly::Uuid const& uuid) const noexcept { return hash<uint64_t>()(uuid.get()); }
};

} // namespace std
