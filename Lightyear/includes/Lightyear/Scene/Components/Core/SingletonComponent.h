#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly::scene {

/**
 * Extend components using this class to indicate that a class is singleton. There will be only one instance of this
 * component across the Entities. Example: MainCamera, EditorCamera. At a time there will only be one camera present.
 *
 * The following template class allows for certain typesafe behaviours and checks to ensure there is only one instance
 * of this class across all entities.
 *
 * Singleton components can be stateless and may be used for indicating specific things in a game world. It can also
 * act as a tag for things such as MainCamera, PlayerCharacter, PlayerController and so on.
 */
template <typename T>
struct SingletonComponent {};

template <typename T>
struct is_singleton_component : std::false_type {};

template <typename T>
struct is_singleton_component<SingletonComponent<T>> : std::true_type {};

template <typename T>
inline constexpr bool is_singleton_tag_v = is_singleton_component<T>::value;

}  // namespace ly::scene
