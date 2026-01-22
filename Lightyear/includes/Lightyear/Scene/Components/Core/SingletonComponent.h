#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly::scene
{

/// @brief Base class to mark a component as a singleton.
///
/// Singleton components ensure that only one instance of the component exists
/// across all entities in a scene. For example, `MainCamera` or `EditorCamera`
/// components should only ever exist once at a time.
///
/// This template provides type-safe checks and enforces the singleton constraint
/// at compile-time or runtime, depending on usage.
///
/// Singleton components may be stateless and are often used as tags to indicate
/// special roles in the game world, such as:
/// - MainCamera
/// - PlayerCharacter
/// - PlayerController
template <typename T>
struct SingletonComponent
{};

template <typename T>
struct IsSingletonComponent : std::false_type
{};

template <typename T>
struct IsSingletonComponent<SingletonComponent<T>> : std::true_type
{};

template <typename T>
inline constexpr bool k_isSingletonTagV = IsSingletonComponent<T>::value;

} // namespace ly::scene
