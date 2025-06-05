#pragma once

#include <xhash>

namespace ly {
class uuid {
public:
    uuid();
    uuid(uint64_t uuid);
    uuid(const uuid&) = default;

    operator uint64_t() const { return m_UUID; }

private:
    uint64_t m_UUID{};
};

}  // namespace ly