#pragma once

#include <lypch.h>
#include "Lightyear/LightyearCore.h"
#include "SceneData.h"
#include "SceneGraph.h"
#include "entt/entt.hpp"

// Forward Declearation
namespace ly::renderer {
class SceneCamera;
}

namespace ly::scene {

// Custom Types
enum class SceneExecState {
    SS_NONE       = 0,
    SS_PAUSED     = 1,
    SS_RUNNING    = 2,
    SS_SIMULATION = 3,
    SS_MAX        = 4
};

// Forward Decleration
class Entity;

class LIGHTYEAR_API Scene {
public:
    Entity CreateEntity(const CName& name);
    Entity CreateEntity(const CName& name, const Entity& parent);
    void DestroyEntity(Entity entity);
    Entity DuplicateEntity(Entity entity);
    Entity FindEntityByName(const CName& name) const;

    Entity CreateChildEntity(Entity parent, const CName& name);
    void AddChildNode(Entity childEntity, Entity newParent);
    void RemoveChildNode(Entity childEntity, Entity parent);

    Entity GetPrimaryCameraEntity() const;

    void OnRuntimeStart();
    void OnRuntimeStop();

    void OnSimulationStart();
    void OnSimulationEnd();

    /**
     * @brief The following update is perfomed on the editor along with games logical layer and
     * using the main camera that is part of the game
     * @param deltaTime the timestep between each frame
     */
    void OnUpdateRuntime(ly::Timestep deltaTime);

    /**
     * @brief Update physics layer and render using sceneCamera
     *
     * @param deltaTime
     * @param camera
     */
    void OnUpdateSimulation(ly::Timestep deltaTime, Ref<renderer::SceneCamera> camera);

    /**
     * @brief The following update is performed on the editor while rendering using the sceneCamera
     * @param deltaTime the time between each frame
     * @param camera the editorial camera
     */
    void OnUpdateEditor(ly::Timestep deltaTime, Ref<renderer::SceneCamera> camera);
    void OnViewportResize(uint32_t width, uint32_t height);

    template <typename... Components>
    auto GetAllEntitiesWith() {
        return m_Registry.view<Components...>();
    }

public:
    inline void SetSceneExecState(SceneExecState state) { m_SceneExecState = state; }
    inline bool IsRunning() const { return m_SceneExecState == SceneExecState::SS_RUNNING; }
    inline bool IsPaused() const { return m_SceneExecState == SceneExecState::SS_PAUSED; }
    inline entt::registry& GetRegistry() { return m_Registry; }
    inline const entt::registry& GetRegistry() const { return m_Registry; }

protected:
    Entity CreateEntity(uuid uuid,
                        const CName& name                                   = std::string(),
                        std::optional<std::reference_wrapper<const Entity>> = std::nullopt);

private:
    entt::registry m_Registry{};
    SceneExecState m_SceneExecState{ SceneExecState::SS_PAUSED };
    uint32_t m_ViewportWidth{};
    uint32_t m_ViewportHeight{};
    std::unordered_map<CName, int> m_EntityNameMap{};
    SceneData m_SceneData{};

private:
    template <typename T>
    void OnComponentAdded(Entity entity, T& component);

    CName GenerateUniqueName(const CName& baseName);

    friend class Entity;
};

}  // namespace ly::scene