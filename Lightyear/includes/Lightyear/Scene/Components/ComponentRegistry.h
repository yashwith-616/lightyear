#pragma once
#include "Lightyear/Common/Macros.h"

LY_DISABLE_WARNINGS_PUSH
#include "cereal/archives/binary.hpp"
#include "cereal/archives/json.hpp"
#include "entt/entt.hpp"
LY_DISABLE_WARNINGS_POP

namespace ly {

template <typename T>
static void emplaceComponent(entt::registry& registry, entt::entity entity, T component) {
    registry.emplace_or_replace<T>(entity, std::move(component));
}

}  // namespace ly

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
                .type(entt::hashed_string{ Name.value })    // hashed type ID
                .data<&T::Name>("name"_hs)                  // static Name property
                .func<&T::SaveJson>("enttSave"_hs)          // static save function
                .func<&T::LoadJson>("enttLoad"_hs)          // static load function
                .func<&emplaceComponent<T>>("emplace"_hs);  // emplace into registry
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

#define SERIALIZE_BODY(T)                                                                                              \
    const char* Name = #T;                                                                                             \
    void SaveJson(cereal::JSONOutputArchive& archive) {                                                                \
        LY_CORE_LOG(LogType::INFO, "Saving Json Called");                                                              \
        serialize(archive);                                                                                            \
    }                                                                                                                  \
    static void LoadJson(cereal::JSONInputArchive& archive, T& component) {                                            \
        LY_CORE_LOG(LogType::INFO, "Loading Json Called");                                                             \
        component.serialize(archive);                                                                                  \
    }