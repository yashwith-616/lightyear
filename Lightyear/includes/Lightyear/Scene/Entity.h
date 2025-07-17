#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Components/Components.h"
#include "Lightyear/Scene/Scene.h"

namespace ly::scene {

/**
 * @brief NEED TO EXTEND THIS CLASS AS GAME OBJECT.
 * All game objects will have relationship component which needs to be custom handled.
 */
class Entity {
public:
    Entity() = default;
    Entity(entt::entity handle, Scene* scene);
    Entity(const Entity& other)            = default;
    Entity(Entity&& other)                 = default;
    Entity& operator=(const Entity& other) = default;

    ~Entity() = default;

    template <typename T, typename... Args>
    T& AddComponent(Args&&... args) {
        LY_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
        T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        m_Scene->OnComponentAdded<T>(*this, component);
        return component;
    }

    template <typename T, typename... Args>
    T& AddOrReplaceComponent(Args&&... args) {
        T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
        m_Scene->OnComponentAdded<T>(*this, component);
        return component;
    }

    template <typename T>
    T& GetComponent() {
        LY_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
        return m_Scene->m_Registry.get<T>(m_EntityHandle);
    }

    template <typename T>
    [[nodiscard]] bool HasComponent() const {
        return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
    }

    template <typename T>
    void RemoveComponent() {
        LY_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
        m_Scene->m_Registry.remove<T>(m_EntityHandle);
    }

    operator bool() const { return m_EntityHandle != entt::null; }
    operator entt::entity() const { return m_EntityHandle; }
    operator uint32_t() const { return static_cast<uint32_t>(m_EntityHandle); }

    UUID GetUUID() { return GetComponent<IDComponent>().ID; }
    const std::string& GetName() { return GetComponent<TagComponent>().Tag; }

    bool operator!=(const Entity& other) const { return !(*this == other); }
    bool operator==(const Entity& other) const {
        return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
    }

private:
    entt::entity m_EntityHandle{ entt::null };
    Scene* m_Scene{ nullptr };
};
}  // namespace ly::scene