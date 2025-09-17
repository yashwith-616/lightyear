#pragma once
#include "Lightyear/Common/Macros.h"

LY_DISABLE_WARNINGS_PUSH
#include "entt/entt.hpp"
LY_DISABLE_WARNINGS_POP

namespace ly::internal {

template <size_t N>
struct templated_string {
    char value[N];

    explicit constexpr templated_string(const char (&str)[N]) {
        for (size_t i = 0; i < N; i++) {
            value[i] = str[i];
        }
    }
};

template <typename T, templated_string Name>
struct ComponentRegistrar {
    static bool RegisterComponent() {
        static bool once = [] {
            using namespace entt::literals;
            entt::meta_factory<T>()
                .type(entt::hashed_string{ Name.value })
                .func<&T::Serialize, entt::as_void_t>("Serialize"_hs)
                .func<&T::Deserialize, entt::as_void_t>("Deserialize"_hs);
            return true;
        }();
        return once;
    }
};

}  // namespace ly::internal

#define CONCAT_IMPL(a, b) a##b

#define CONCAT(a, b) CONCAT_IMPL(a, b)

#define REGISTER_COMPONENT(T, Name)                                                                                    \
    [[maybe_unused]] inline bool CONCAT(reg_component_, __COUNTER__) =                                                 \
        ly::internal::ComponentRegistrar<T,                                                                            \
                                         ly::internal::templated_string<sizeof(Name)>{ Name }>::RegisterComponent();