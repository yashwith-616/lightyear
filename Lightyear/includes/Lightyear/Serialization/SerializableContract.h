#pragma once

// === Types ==================================================

namespace ly {

class StreamReader;
class StreamWriter;

struct SerializableContract {};
}  // namespace ly

// === Concepts ==================================================

template <typename T>
concept HasSerializeContract = std::is_base_of_v<ly::SerializableContract, T>;

template <typename T>
concept HasVersion = requires {
    { T::version } -> std::same_as<const ly::Version>;
};

template <typename T>
concept HasSerialize = requires(const T& obj, ly::StreamWriter& serializer) {
    { T::Serialize(serializer, obj) } -> std::same_as<void>;
};

template <typename T>
concept HasDeserialize = requires(T& obj, ly::StreamReader& deserializer) {
    { T::Deserialize(deserializer, obj) } -> std::same_as<void>;
};
