#pragma once

#include <type_traits>
#include "Lightyear/Serialization/SerializationAdapters.h"

// === Types ==================================================
namespace ly {

class StreamReader;
class StreamWriter;

class TextSerialization;
class ITextReader;

struct Version;

}  // namespace ly

// === Concepts ==================================================
namespace ly::internal {

template <typename T>
concept has_serialize_contract_v = std::is_base_of_v<SerializableContract, T>;

template <typename T>
concept has_version_v = requires {
    { T::version } -> std::same_as<const Version>;
};

template <typename T>
concept has_serialize_v = requires(const T& obj, StreamWriter& serializer) {
    { T::Serialize(serializer, obj) } -> std::same_as<void>;
};

template <typename T>
concept has_deserialize_v = requires(T& obj, StreamReader& deserializer) {
    { T::Deserialize(deserializer, obj) } -> std::same_as<void>;
};

template <typename T>
concept has_text_serialize_v = requires(const T& obj, TextSerialization& serializer) {
    { T::Serialize(serializer, obj) } -> std::same_as<void>;
};

template <typename T>
concept has_text_deserialize_v = requires(T& obj, ITextReader& deserializer) {
    { T::Deserialize(deserializer, obj) } -> std::same_as<void>;
};

template <typename T, typename = void>
struct is_primitive_serializable : std::false_type {};

template <typename T>
struct is_primitive_serializable<T, std::void_t<decltype(SerializableAdapter<T>::value)>>
    : std::bool_constant<SerializableAdapter<T>::value> {};

/// \brief Checks if the template is of container type
template <typename T>
concept is_container_type_v = requires(T a) {
    std::begin(a);
    std::end(a);
} && !std::is_same_v<T, std::string>;

}  // namespace ly::internal
