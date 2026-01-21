#pragma once

#include "Lightyear/Common/Math.h"
#include "Lightyear/pch/lypch.h"

constexpr int k_openglMajorVersion{4};
constexpr int k_openglMinorVersion{6};

namespace ly
{

// ----------------------- Constants ---------------------------
// TO BE MOVED: Windows properties
constexpr glm::uvec2 k_defaultWindowSize{1280, 720};
constexpr float k_kDefaultFrametime{1 / 60.f};
constexpr std::string_view k_defaultWindowTitle{"LightYear"};
constexpr std::string_view k_kGlslVersion = "#version 460 core";
constexpr std::string_view k_kNotset{"NOT_SET"};

constexpr float k_kDefaultAspectRatio{1.7777777778f};
constexpr float k_kDefaultFovRadians{glm::radians(45.f)};
constexpr float k_kDefaultNearClip{0.1f};
constexpr float k_kDefaultFarClip{100.f};
constexpr float k_kOrthographicCameraSize{10.f};

// ----------------------- Custom ------------------------------
constexpr uint8_t bit(uint8_t const x) { return 1u << x; }

inline void const* asVoidPtr(uint32_t const value)
{
    return reinterpret_cast<const void*>(static_cast<uintptr_t>(value)); // NOLINT
}

// ----------------------- Callbacks ---------------------------
using eventCallbackFn = std::function<void(class Event&)>;

using cPath = std::filesystem::path;

} // namespace ly

#include "Lightyear/Common/Base.inl"
