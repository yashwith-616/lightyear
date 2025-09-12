#pragma once

#include <type_traits>
#include "Lightyear/Serialization/SerializationAdapters.h"

// === Types ==================================================
namespace ly {

class BinaryFileDeserializer;
class BinaryFileSerializer;

class TextSerializer;
class TextDeserializer;

struct Version;

}  // namespace ly

// === Concepts ==================================================
namespace ly::internal {

template <typename T>
concept has_serialize_contract_v = std::is_base_of_v<SerializableContract, T>;

template <typename T>
concept has_version_v = requires {
    { T::version } -> std::convertible_to<Version>;
};

template <typename T>
concept has_serialize_v = requires(const T& obj, BinaryFileSerializer& serializer) {
    { T::Serialize(serializer, obj) } -> std::same_as<void>;
};

template <typename T>
concept has_deserialize_v = requires(T& obj, BinaryFileDeserializer& deserializer) {
    { T::Deserialize(deserializer, obj) } -> std::same_as<void>;
};

template <typename T>
concept has_text_serialize_v = requires(const T& obj, TextSerializer& serializer) {
    { T::Serialize(serializer, obj) } -> std::same_as<void>;
};

template <typename T>
concept has_text_deserialize_v = requires(T& obj, TextDeserializer& deserializer) {
    { T::Deserialize(deserializer, obj) } -> std::same_as<void>;
};

template <typename T>
concept is_primitive_serializable = requires { SerializableAdapter<T>::value; } && SerializableAdapter<T>::value;

/// \brief Checks if the template is of container type
template <typename T>
concept is_container_type_v = requires(T a) {
    std::begin(a);
    std::end(a);
} && !std::is_same_v<T, std::string>;

template <typename T>
concept has_migrate = requires(uint64_t v) {
    { T::Migrate(v) };
};

}  // namespace ly::internal
