#pragma once

#include "Lightyear/pch/lypch.h"

namespace ly {

// ----------------------- Custom Memory -----------------------

/**
 * Wrapper around unique_pointer
 * @tparam T the type
 */
template <typename T>
using Scope = std::unique_ptr<T>;

/**
 * Wrapper around make_unique
 * @tparam T the Type
 * @tparam Args the constructor arguments
 * @param args the constructor arguments
 * @return unique pointer
 */
template <typename T, typename... Args>
constexpr std::unique_ptr<T> MakeScope(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

/**
 * Wrapper around shared_pointer
 * @tparam T the type
 */
template <typename T>
using Ref = std::shared_ptr<T>;

/**
 * Wrapper around make_shared.
 * @tparam T the type
 * @tparam Args the constructor arguments
 * @param args the constructor arguments
 * @return shared pointer
 */
template <typename T, typename... Args>
constexpr std::shared_ptr<T> MakeRef(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

/**
 * Wrapper around the weak pointers
 * @tparam T the type
 */
template <typename T>
using WeakRef = std::weak_ptr<T>;

/**
 * Acquire lock on a weak pointer. Converts weak pointer to shared
 * @tparam T the type
 */
template <typename T>
Ref<T> TryLock(const WeakRef<T>& weak) {
    return weak.lock();
}

// ----------------------- Casting -----------------------

/**
 * The following cast is used as an identifier across the codebase to identify all narrow casts. Allows to easily
 * search for narrowing operations.
 *
 * @tparam T cast from type
 * @tparam U cast to type
 * @param value the value to be cast
 * @return value after cast
 */
template <typename T, typename U>
constexpr T narrow_cast(U value) {
    return static_cast<T>(value);
}

// ----------------------- Types -----------------------
/// @brief The following type is used across engine for identifying versions
///
struct Version {
    int16_t version{ 0 };

    constexpr explicit Version(int16_t version) : version(version) {}

    bool operator==(const Version& other) const noexcept { return this->version == other.version; }
    bool operator!=(const Version& other) const noexcept { return this->version != other.version; }
    bool operator<(const Version& other) const noexcept { return this->version < other.version; }
    bool operator>(const Version& other) const noexcept { return this->version > other.version; }
    bool operator<=(const Version& other) const noexcept { return this->version <= other.version; }
    bool operator>=(const Version& other) const noexcept { return this->version >= other.version; }

    [[nodiscard]] constexpr int16_t get() const noexcept { return version; }
};

/// \brief The window size in lightyear
struct WindowSize {
    int32_t width{ 0 };
    int32_t height{ 0 };
};

/// \brief The graphic context version
struct GraphicContextVersion {
    int majorVersion{ 0 };
    int minorVersion{ 0 };
};

template <typename T>
constexpr std::string_view type_name() {
#ifdef __clang__
    constexpr std::string_view p = __PRETTY_FUNCTION__;
    return p.substr(31, p.size() - 32);
#elif defined(__GNUC__)
    constexpr std::string_view p = __PRETTY_FUNCTION__;
    return p.substr(46, p.size() - 47);
#elif defined(_MSC_VER)
    constexpr std::string_view p = __FUNCSIG__;
    return p.substr(38, p.size() - 45);
#endif
}

/// \brief Enables easy use of RAII wrappers using lambda
template <typename F>
struct ScopeGuard {
    F func;
    explicit ScopeGuard(F f) : func(f) {}
    ~ScopeGuard() { func(); }
};

/// \brief Provides an easier way to implement RAII guards
template <typename F>
ScopeGuard<F> MakeScopeGuard(F f) {
    return ScopeGuard<F>(f);
}

}  // namespace ly