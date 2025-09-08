#pragma once

namespace ly {

struct SerializableContract {};

template <typename T, typename = void>
struct SerializableAdapter {
    static constexpr bool value = false;
};

}  // namespace ly
