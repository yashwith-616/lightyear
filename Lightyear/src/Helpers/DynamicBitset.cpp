#include "Lightyear/Helpers/DynamicBitset.h"

namespace ly {

DynamicBitset::DynamicBitset(size_t size) {
    Resize(size);
}

void DynamicBitset::Resize(size_t size) {
    const size_t blocks = BlockCount(size);
    if (blocks > m_Bits.size()) {
        m_Bits.resize(blocks, 0);
    }
    m_Size = size;
}

void DynamicBitset::Set(size_t index, bool value) {
    LY_CORE_ASSERT(m_Size > index, "DynamicBitset::Set - Index out of range");

    const size_t block = index / kBitsPerBlock;
    const size_t bit   = index % kBitsPerBlock;

    if (value) {
        m_Bits[block] |= (1ULL << bit);
    } else {
        m_Bits[block] &= ~(1ULL << bit);
    }
}

void DynamicBitset::Reset(size_t index) {
    Set(index, false);
}

void DynamicBitset::Flip(size_t index) {
    LY_CORE_ASSERT(m_Size > index, "DynamicBitset::Flip - Index out of range");

    const size_t block = index / kBitsPerBlock;
    const size_t bit   = index % kBitsPerBlock;
    m_Bits[block] ^= (1ULL << bit);
}

bool DynamicBitset::Test(size_t index) const {
    LY_CORE_ASSERT(m_Size > index, "DynamicBitset::Test - Index out of range");

    const size_t block = index / kBitsPerBlock;
    const size_t bit   = index % kBitsPerBlock;
    return static_cast<bool>((m_Bits[block] >> bit) & 1ULL);
}

}  // namespace ly