#pragma once

#include <lypch.h>
#include "Lightyear/LightyearCore.h"

namespace ly {

class dynamic_bitset {
public:
    dynamic_bitset() = default;
    explicit dynamic_bitset(size_t size);

    void resize(size_t size);
    void set(size_t index, bool value = true);
    void reset(size_t index);
    void flip(size_t index);
    bool test(size_t index) const;
    size_t size() const { return m_Size; }

private:
    std::vector<uint64_t> m_Bits;
    size_t m_Size = 0;

    static constexpr size_t BITS_PER_BLOCK = 64;
    size_t block_count(size_t size) { return (size + BITS_PER_BLOCK - 1) / BITS_PER_BLOCK; }
};
}  // namespace ly