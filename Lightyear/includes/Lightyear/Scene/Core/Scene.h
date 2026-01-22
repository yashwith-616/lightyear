#pragma once

#include "Lightyear/LightyearCore.h"

LY_DISABLE_WARNINGS_PUSH
#include "entt/entt.hpp"
LY_DISABLE_WARNINGS_POP

namespace ly::scene
{

class Entity;

class LIGHTYEAR_API Scene
{
public:
    Scene();
    Scene(Scene const&) = delete;
    Scene(Scene&&) noexcept = default;
    Scene& operator=(Scene const&) = delete;
    Scene& operator=(Scene&&) = default;

    Entity createEntity(std::string const& name);
    Entity createEntity(std::string const& name, Entity const& parent);
    void destroyEntity(Entity entity);
    Entity duplicateEntity(Entity entity);
    [[nodiscard]] Entity findEntityByName(std::string const& name) const;

    Entity createChildEntity(Entity parent, std::string const& name);
    void addChildNode(Entity childEntity, Entity newParent);
    void removeChildNode(Entity childEntity, Entity parent);

    [[nodiscard]] Entity getPrimaryCameraEntity() const;

    template <typename... Components>
    auto getAllEntitiesWith()
    {
        return m_registry.view<Components...>();
    }

    [[nodiscard]] entt::registry& getRegistry() { return m_registry; }
    [[nodiscard]] entt::registry const& getRegistry() const { return m_registry; }

protected:
    Entity createEntity(
        Uuid uuid,
        std::string const& name = std::string(),
        std::optional<std::reference_wrapper<Entity const>> = std::nullopt);

private:
    entt::registry m_registry;
    std::unordered_map<std::string, uint32_t> m_entityNameMap;

    std::string generateUniqueName(std::string const& baseName);

    static void onTransformUpdated(entt::registry& registry, entt::entity entity);
    static void onCameraUpdated(entt::registry& registry, entt::entity entity);

    /*template <typename T>
    void Scene::OnComponentAdded(Entity /*entity#1#, T& /*component#1#) {
        LY_CORE_LOG(ly::LogType::WARN, "Scene::OnComponentAdded: Generic handler for component type {}",
    typeid(T).name());

        if constexpr (std::is_empty_v<T>) {
            LY_CORE_LOG(ly::LogType::WARN, "Component is an empty type");
        }
    }*/

    friend class Entity;
};

} // namespace ly::scene
