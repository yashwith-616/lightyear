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
    Scene(const Scene&) = delete;
    Scene(Scene&&) noexcept = default;
    Scene& operator=(const Scene&) = delete;
    Scene& operator=(Scene&&) = default;

    Entity CreateEntity(const std::string& name);
    Entity CreateEntity(const std::string& name, const Entity& parent);
    void DestroyEntity(Entity entity);
    Entity DuplicateEntity(Entity entity);
    [[nodiscard]] Entity FindEntityByName(const std::string& name) const;

    Entity CreateChildEntity(Entity parent, const std::string& name);
    void AddChildNode(Entity childEntity, Entity newParent);
    void RemoveChildNode(Entity childEntity, Entity parent);

    [[nodiscard]] Entity GetPrimaryCameraEntity() const;

    template <typename... Components>
    auto GetAllEntitiesWith()
    {
        return m_Registry.view<Components...>();
    }

    [[nodiscard]] entt::registry& GetRegistry() { return m_Registry; }
    [[nodiscard]] const entt::registry& GetRegistry() const { return m_Registry; }

protected:
    Entity CreateEntity(
        Uuid UUID,
        const std::string& name = std::string(),
        std::optional<std::reference_wrapper<const Entity>> = std::nullopt);

private:
    entt::registry m_Registry;
    std::unordered_map<std::string, uint32_t> m_EntityNameMap;

    std::string GenerateUniqueName(const std::string& baseName);

    static void OnTransformUpdated(entt::registry& registry, entt::entity entity);
    static void OnCameraUpdated(entt::registry& registry, entt::entity entity);

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
