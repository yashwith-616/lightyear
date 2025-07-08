#pragma once

#include "Lightyear/pch/lypch.h"

namespace ly {

// ----------------------- Constants ---------------------------
// TO BE MOVED: Windows properties
constexpr uint32_t DEFAULT_WINDOW_HEIGHT{ 720 };
constexpr uint32_t DEFAULT_WINDOW_WIDTH{ 1280 };
constexpr std::string_view DEFAULT_WINDOW_TITLE{ "LightYear" };
constexpr float DEFAULT_FRAMETIME{ 1 / 60.f };
constexpr float DEFAULT_ASPECT_RATIO{ 1.7777777778f };
constexpr std::string_view GLSL_VERSION = "#version 460 core";

// ----------------------- Custom ------------------------------
constexpr uint8_t BIT(uint8_t x) {
    return 1 << x;
}

inline const void* AsVoidPtr(uint32_t value) {
    return reinterpret_cast<const void*>(static_cast<uintptr_t>(value));
}

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
inline Ref<T> TryLock(const WeakRef<T>& weak) {
    return weak.lock();
}

// ----------------------- Callbacks ---------------------------
using EventCallbackFn = std::function<void(class Event&)>;

using CName = std::string;
using CText = std::string;
using CPath = std::filesystem::path;

}  // namespace ly
