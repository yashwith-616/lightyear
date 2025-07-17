#pragma once

#include "Lightyear/pch/lypch.h"

constexpr int OPENGL_MAJOR_VERSION{ 4 };
constexpr int OPENGL_MINOR_VERSION{ 6 };

namespace ly {

// ----------------------- Constants ---------------------------
// TO BE MOVED: Windows properties
constexpr uint32_t kDefaultWindowHeight{ 720 };
constexpr uint32_t kDefaultWindowWidth{ 1280 };
constexpr std::string_view kDefaultWindowTitle{ "LightYear" };
constexpr float kDefaultFrametime{ 1 / 60.f };
constexpr float kDefaultAspectRatio{ 1.7777777778f };
constexpr std::string_view kGLSLVersion = "#version 460 core";

// ----------------------- Custom ------------------------------
constexpr uint8_t BIT(const uint8_t x) {
    return 1u << x;
}

inline const void* AsVoidPtr(const uint32_t value) {
    return reinterpret_cast<const void*>(static_cast<uintptr_t>(value));
}

// ----------------------- Callbacks ---------------------------
using EventCallbackFn = std::function<void(class Event&)>;

using CPath = std::filesystem::path;

}  // namespace ly

#include "Lightyear/Common/Base.inl"