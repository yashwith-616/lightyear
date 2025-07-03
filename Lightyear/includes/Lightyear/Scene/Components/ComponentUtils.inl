#pragma once

#include "ComponentsForward.h"

namespace ly::scene {

/**
 * @brief Group components together, Allowes to pass components in
 * @tparam ...Component List of components
 */
template <typename... Component>
struct ComponentGroup {
    using Types = std::tuple<Component...>;
};

template <typename Group>
struct ComponentGroupView;

/**
 * @brief Perform an view operation on entt::view
 *
 * @tparam ...Components The list of components
 */
template <typename... Components>
struct ComponentGroupView<ComponentGroup<Components...>> {
    template <typename Registry>
    static auto view(Registry& registry) {
        return registry.view<Components...>();
    }
};

template <typename Group>
struct ComponentGroupGet;

/**
 * @brief Perform entt get operation on a component group
 *
 * @tparam ...Components Group create Component Group using ComponentGroup
 */
template <typename... Components>
struct ComponentGroupGet<ComponentGroup<Components...>> {
    template <typename Registry, typename Entity>
    static auto get(Registry& registry, Entity entity) {
        return registry.get<Components...>(entity);
    }
};

/**
 * @brief All Components present in Lightyear
 */
using AllComponents = ComponentGroup<TransformComponent,
                                     CameraComponent,
                                     MobilityComponent,
                                     MeshComponent,
                                     RenderComponent,
                                     RelationshipComponent>;

}  // namespace ly::scene
