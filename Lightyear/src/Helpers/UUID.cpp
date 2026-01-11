#include "Lightyear/Helpers/UUID.h"

namespace ly
{

Uuid::Uuid() : m_uuid(getUuidGenerator().generate()) {}

Uuid::Uuid(uint64_t uuid) : m_uuid(uuid) {}

} // namespace ly

template <>
struct std::hash<ly::Uuid>
{
    std::size_t operator()(const ly::Uuid& uuid) const noexcept { return hash<uint64_t>()(uuid.get()); }
}; // namespace std
