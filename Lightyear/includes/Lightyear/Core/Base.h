#pragma once

#include <lypch.h>
#include <glm/glm.hpp>
#include "spdlog/fmt/fmt.h"

namespace ly {

// ----------------------- Constants ---------------------------
constexpr uint32_t DEFAULT_WINDOW_HEIGHT{ 720 };
constexpr uint32_t DEFAULT_WINDOW_WIDTH{ 1280 };
constexpr std::string_view DEFAULT_WINDOW_TITLE{ "LightYear" };
constexpr float DEFAULT_FRAMETIME{ 1 / 60.f };

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

// ----------------------- GLM Debug Support -----------------------
namespace fmt {

template <>
struct formatter<glm::vec4> {
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const glm::vec4& v, FormatContext& ctx) const {
        return format_to(ctx.out(), "vec4({}, {}, {}, {})", v.x, v.y, v.z, v.w);
    }
};

template <>
struct formatter<glm::mat4> {
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const glm::mat4& m, FormatContext& ctx) const {
        return format_to(
            ctx.out(),
            "mat4([\n  {}, {}, {}, {}\n  {}, {}, {}, {}\n  {}, {}, {}, {}\n  {}, {}, {}, {}\n])",
            m[0][0],
            m[0][1],
            m[0][2],
            m[0][3],
            m[1][0],
            m[1][1],
            m[1][2],
            m[1][3],
            m[2][0],
            m[2][1],
            m[2][2],
            m[2][3],
            m[3][0],
            m[3][1],
            m[3][2],
            m[3][3]);
    }
};

}  // namespace fmt
