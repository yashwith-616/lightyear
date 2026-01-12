#pragma once
#include "Lightyear/Common/Macros.h"

LY_DISABLE_WARNINGS_PUSH
#include "cereal/archives/binary.hpp"
#include "cereal/archives/json.hpp"
#include "entt/entt.hpp"
LY_DISABLE_WARNINGS_POP

namespace ly {

template <typename T>
static void emplaceComponent(entt::registry& registry, entt::entity entity, entt::meta_any instance) {
    T obj = instance.cast<T>();
    registry.emplace_or_replace<T>(entity, obj);
}

}  // namespace ly

namespace ly::internal {

template <size_t N>
struct TemplatedString {
    char value[N];

    explicit constexpr TemplatedString(char const (&str)[N]) {
        for (size_t i = 0; i < N; i++) {
            value[i] = str[i];
        }
    }
};

template <typename T, TemplatedString Name>
struct ComponentRegistrar {
    static bool registerComponent() {
        static bool once = [] {
            using namespace entt::literals;

            entt::meta_factory<T>()
                .type(entt::hashed_string{ Name.value })
                .template data<&T::Name>("name"_hs)
                .template func<&T::saveJson>("enttSave"_hs)
                .template func<&T::loadJson>("enttLoad"_hs)
                .template func<&emplaceComponent<T>>("emplace"_hs);
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
        ly::internal::ComponentRegistrar<T, ly::internal::TemplatedString<sizeof(Name)>{ Name }>::registerComponent();

#define SERIALIZE_BODY(T)                                                                                              \
    const char* Name = #T;                                                                                             \
    void saveJson(cereal::JSONOutputArchive& archive) {                                                                \
        save(archive);                                                                                                 \
    }                                                                                                                  \
    static void loadJson(cereal::JSONInputArchive& archive, T& component) {                                            \
        component.load(archive);                                                                                       \
    }