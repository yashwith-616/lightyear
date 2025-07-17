#pragma once

#include "Lightyear/pch/lypch.h"

namespace ly {

// ----------------------- Custom Memory -----------------------

template <typename T>
using Scope = std::unique_ptr<T>;

template <typename T, typename... Args>
constexpr std::unique_ptr<T> MakeScope(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}


template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
constexpr std::shared_ptr<T> MakeRef(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}


template <typename T>
using WeakRef = std::weak_ptr<T>;

template <typename T>
Ref<T> TryLock(const WeakRef<T>& weak) {
    return weak.lock();
}

// ----------------------- Casting -----------------------
template<typename T, typename U>
constexpr T narrow_cast(U value) {
    return static_cast<T>(value);
}

} // namespace ly