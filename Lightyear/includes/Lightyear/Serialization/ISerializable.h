#pragma once

#include "Lightyear/Serialization/StreamReader.h"
#include "Lightyear/Serialization/StreamWriter.h"

// -- Detection idiom helpers --
template <typename T, typename = void>
struct has_serialize : std::false_type {};

template <typename T>
struct has_serialize<T, std::void_t<decltype(T::Serialize(std::declval<ly::StreamWriter*>(), std::declval<T&>()))>>
    : std::true_type {};

template <typename T, typename = void>
struct has_deserialize : std::false_type {};

template <typename T>
struct has_deserialize<T, std::void_t<decltype(T::Deserialize(std::declval<ly::StreamReader*>(), std::declval<T&>()))>>
    : std::true_type {};

// -- Interface --
namespace ly {

template <typename Derived>
struct ISerializable {
    static constexpr int Version = Derived::Version;

    static void Serialize(StreamWriter* serializer, const Derived& derived) {
        static_assert(has_serialize<Derived>::value, "Serialization is not implemented");
        Derived::Serialize(serializer, derived);
    }

    static void Deserialize(StreamReader* deserializer, Derived& derived) {
        static_assert(has_deserialize<Derived>::value, "Deserialization is not implemented");
        Derived::Deserialize(deserializer, derived);
    }

protected:
    ISerializable() = default;
};

}  // namespace ly
