#include "Lightyear/Helpers/DynamicBitset.h"
#include "Lightyear/Common/Assertions.h"

namespace ly
{

DynamicBitset::DynamicBitset(size_t size) { resize(size); }

void DynamicBitset::resize(size_t size)
{
    size_t blocks = blockCount(size);
    if (blocks > m_bits.size())
    {
        m_bits.resize(blocks, 0);
    }
    m_size = size;
}

void DynamicBitset::set(size_t index, bool value)
{
    LY_CORE_ASSERT(m_size > index, "DynamicBitset::Set - Index out of range");

    size_t block = index / k_bitsPerBlock;
    size_t bit = index % k_bitsPerBlock;

    if (value)
    {
        m_bits[block] |= (1ULL << bit);
    }
    else
    {
        m_bits[block] &= ~(1ULL << bit);
    }
}

void DynamicBitset::reset(size_t index) { set(index, false); }

void DynamicBitset::flip(size_t index)
{
    LY_CORE_ASSERT(m_size > index, "DynamicBitset::Flip - Index out of range");

    size_t block = index / k_bitsPerBlock;
    size_t bit = index % k_bitsPerBlock;
    m_bits[block] ^= (1ULL << bit);
}

bool DynamicBitset::test(size_t index) const
{
    LY_CORE_ASSERT(m_size > index, "DynamicBitset::Test - Index out of range");

    size_t block = index / k_bitsPerBlock;
    size_t bit = index % k_bitsPerBlock;
    return static_cast<bool>((m_bits[block] >> bit) & 1ULL);
}

} // namespace ly
