#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Components/Components.h"
#include "Lightyear/Scene/Core/Scene.h"

namespace ly::scene
{

/**
 * @brief NEED TO EXTEND THIS CLASS AS GAME OBJECT.
 * All game objects will have relationship component which needs to be custom handled.
 */
class Entity
{
public:
    Entity() = default;
    Entity(entt::entity handle, Scene* scene);
    Entity(Entity const& other) = default;
    Entity(Entity&& other) = default;
    Entity& operator=(Entity const& other) = default;

    ~Entity() = default;

    template <typename T, typename... Args>
    T& addComponent(Args&&... args)
    {
        LY_CORE_ASSERT(!hasComponent<T>(), "Entity already has component!");
        T& component = getRegistry().emplace<T>(m_entityHandle, std::forward<Args>(args)...);
        // m_Scene->OnComponentAdded<T>(*this, component);
        return component;
    }

    template <typename T>
    void addSingletonComponent()
    {
        LY_CORE_ASSERT(!hasComponent<T>(), "Entity already has component!");
        auto view = getRegistry().view<T>();
        LY_ASSERT(view.size() <= 1, "More than one SINGLETON COMPONENT exists!");
        for (entt::entity entity : view)
        {
            if (entity != m_entityHandle)
            {
                getRegistry().remove<T>(entity);
            }
        }

        if (!getRegistry().all_of<T>(m_entityHandle))
        {
            getRegistry().emplace<T>(m_entityHandle);
        }
    }

    template <typename T, typename... Args>
    T& addOrReplaceComponent(Args&&... args)
    {
        T& component = m_scene->m_registry.emplace_or_replace<T>(m_entityHandle, std::forward<Args>(args)...);
        // m_Scene->OnComponentAdded<T>(*this, component);
        return component;
    }

    template <typename T>
    T& getComponent()
    {
        LY_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
        return getRegistry().get<T>(m_entityHandle);
    }

    template <typename T>
    [[nodiscard]] bool hasComponent() const
    {
        return getRegistry().all_of<T>(m_entityHandle);
    }

    template <typename T>
    void removeComponent() const
    {
        LY_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
        getRegistry().remove<T>(m_entityHandle);
    }

    template <typename T>
    void sendUpdateSignal() const
    {
        LY_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
        getRegistry().patch<T>(m_entityHandle, [](auto&) {});
    }

    operator bool() const { return m_entityHandle != entt::null; }
    operator entt::entity() const { return m_entityHandle; }
    operator uint32_t() const { return static_cast<uint32_t>(m_entityHandle); }

    Uuid getUuid() { return getComponent<IdComponent>().id; }
    std::string const& getName() { return getComponent<TagComponent>().tag; }

    bool operator!=(Entity const& other) const { return !(*this == other); }
    bool operator==(Entity const& other) const
    {
        return m_entityHandle == other.m_entityHandle && m_scene == other.m_scene;
    }

private:
    entt::entity m_entityHandle{entt::null};
    Scene* m_scene{nullptr};

    FORCEINLINE entt::registry& getRegistry() const { return m_scene->m_registry; }
};
} // namespace ly::scene
