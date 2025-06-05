#include "Lightyear/Helpers/dynamic_bitset.h"

namespace ly {

dynamic_bitset::dynamic_bitset(size_t size) {
    resize(size);
}

void dynamic_bitset::resize(size_t size) {
    size_t blocks = block_count(size);
    if (blocks > m_Bits.size()) {
        m_Bits.resize(blocks, 0);
    }
    m_Size = size;
}

void dynamic_bitset::set(size_t index, bool value) {
    LY_CORE_ASSERT(m_Size > index, "dynamic_bitset::set - Index out of range");

    const size_t block = index / BITS_PER_BLOCK;
    const size_t bit   = index % BITS_PER_BLOCK;

    if (value) {
        m_Bits[block] |= (1ULL << bit);
    } else {
        m_Bits[block] &= ~(1ULL << bit);
    }
}

void dynamic_bitset::reset(size_t index) {
    set(index, false);
}

void dynamic_bitset::flip(size_t index) {
    LY_CORE_ASSERT(m_Size > index, "dynamic_bitset::flip - Index out of range");

    const size_t block = index / BITS_PER_BLOCK;
    const size_t bit   = index % BITS_PER_BLOCK;
    m_Bits[block] ^= (1ULL << bit);
}

bool dynamic_bitset::test(size_t index) const {
    LY_CORE_ASSERT(m_Size > index, "dynamic_bitset::test - Index out of range");

    const size_t block = index / BITS_PER_BLOCK;
    const size_t bit   = index % BITS_PER_BLOCK;
    return (m_Bits[block] >> bit) & 1ULL;
}

}  // namespace ly