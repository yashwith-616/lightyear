#pragma once

#include <memory>

constexpr uint8_t BIT(uint8_t x) {
    return 1 << x;
}

inline const void* TO_VOID_PTR(uint32_t value) {
    return reinterpret_cast<const void*>(static_cast<uintptr_t>(value));
}

namespace ly {

template <typename T>
using Scope = std::unique_ptr<T>;

template <typename T>
using Ref = std::shared_ptr<T>;

}  // namespace ly