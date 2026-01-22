#pragma once
#include <vector>

namespace ly
{

class DynamicBitset
{
public:
    DynamicBitset() = default;
    explicit DynamicBitset(size_t size);

    void resize(size_t size);
    void set(size_t index, bool value = true);
    void reset(size_t index);
    void flip(size_t index);
    [[nodiscard]] bool test(size_t index) const;
    [[nodiscard]] size_t size() const { return m_size; }

private:
    std::vector<uint64_t> m_bits;
    size_t m_size = 0;

    static constexpr size_t k_bitsPerBlock = 64;
    static constexpr size_t blockCount(size_t size) { return (size + k_bitsPerBlock - 1) / k_bitsPerBlock; }
};

} // namespace ly
