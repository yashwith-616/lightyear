#pragma once

#include <lypch.h>
#include <glm/glm.hpp>

namespace ly {

// ----------------------- Constants ---------------------------
constexpr uint32_t DEFAULT_WINDOW_HEIGHT{ 720 };
constexpr uint32_t DEFAULT_WINDOW_WIDTH{ 1280 };
constexpr std::string_view DEFAULT_WINDOW_TITLE{ "LightYear" };
constexpr float DEFAULT_FRAMETIME{ 1 / 60.f };
constexpr float DEFAULT_ASPECT_RATIO{ 1.7777777778f };
constexpr std::string_view GLSL_VERSION = "#version 460 core";

constexpr uint8_t BIT(uint8_t x) {
    return 1 << x;
}

inline const void* TO_VOID_PTR(uint32_t value) {
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

// ----------------------- Custom Strings ----------------------
using CName   = std::string;
using CParam  = std::string_view;
using CLabel  = std::string_view;
using CTag    = std::string_view;
using CId     = std::string;
using CText   = std::string;
using CPath   = std::filesystem::path;
using CShader = std::string_view;

// ----------------------- Callbacks ---------------------------
using EventCallbackFn = std::function<void(class Event&)>;

}  // namespace ly
