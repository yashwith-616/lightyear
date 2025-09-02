#pragma once

#include "Lightyear/pch/lypch.h"

constexpr int OPENGL_MAJOR_VERSION{ 4 };
constexpr int OPENGL_MINOR_VERSION{ 6 };

namespace ly {

// ----------------------- Constants ---------------------------
// TO BE MOVED: Windows properties
constexpr glm::uvec2 kDefaultWindowSize{ 1280, 720 };
constexpr float kDefaultFrametime{ 1 / 60.f };
constexpr std::string_view kDefaultWindowTitle{ "LightYear" };
constexpr std::string_view kGLSLVersion = "#version 460 core";
constexpr std::string_view kNOTSET{ "NOT_SET" };

constexpr float kDefaultAspectRatio{ 1.7777777778f };
constexpr float kDefaultFOVRadians{ glm::radians(45.f) };
constexpr float kDefaultNearClip{ 0.1f };
constexpr float kDefaultFarClip{ 100.f };
constexpr float kOrthographicCameraSize{ 10.f };

// ----------------------- Custom ------------------------------
constexpr uint8_t BIT(const uint8_t x) {
    return 1u << x;
}

inline const void* AsVoidPtr(const uint32_t value) {
    return reinterpret_cast<const void*>(static_cast<uintptr_t>(value)); // NOLINT
}

// ----------------------- Callbacks ---------------------------
using EventCallbackFn = std::function<void(class Event&)>;

using CPath = std::filesystem::path;

}  // namespace ly

#include "Lightyear/Common/Base.inl"