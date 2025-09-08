#pragma once

namespace ly {

class TextSerializer;
class TextDeserializer;

struct SerializableContract {};

template <typename T, typename = void>
struct SerializableAdapter {
    static constexpr bool value = false;
};

}  // namespace ly
