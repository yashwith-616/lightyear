#pragma once

#include "Lightyear/Core/Assertions.h"
#include "Lightyear/pch/lypch.h"

namespace ly {

class DynamicBitset {
public:
    DynamicBitset() = default;
    explicit DynamicBitset(size_t size);

    void Resize(size_t size);
    void Set(size_t index, bool value = true);
    void Reset(size_t index);
    void Flip(size_t index);
    [[nodiscard]] bool Test(size_t index) const;
    [[nodiscard]] size_t Size() const { return m_Size; }

private:
    std::vector<uint64_t> m_Bits;
    size_t m_Size = 0;

    static constexpr size_t kBitsPerBlock = 64;
    static constexpr size_t BlockCount(size_t size) {
        return (size + kBitsPerBlock - 1) / kBitsPerBlock;
    }
};

}  // namespace ly
