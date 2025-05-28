#pragma once

#include <lypch.h>
#include "Lightyear/LightyearCore.h"
#include "SceneGraph.h"
#include "entt/entt.hpp"

namespace ly::scene {

class Entity;
class EditorCamera;

enum SceneExecState { SS_PAUSED = 0, SS_RUNNING = 1, SS_MAX = 2 };

class LIGHTYEAR_API Scene {
public:
    Entity CreateEntity(const CName& name);
    Entity CreateEntity(uuid uuid, const CName& name = std::string());
    void DestroyEntity(Entity entity);
    Entity DuplicateEntity(Entity entity);
    Entity FindEntityByName(const CName& name) const;
    Entity GetPrimaryCameraEntity() const;

    void OnRuntimeStart();
    void OnRuntimeStop();

    void OnSimulationStart();
    void OnSimulationEnd();

    void OnUpdateRuntime(ly::Timestep deltaTime);
    void OnUpdateSimulation(ly::Timestep deltaTime, EditorCamera& camera);
    void OnUpdateEditor(ly::Timestep deltaTime, EditorCamera& camera);
    void OnViewportResize(uint32_t width, uint32_t height);

    bool IsRunning() const { return m_SceneExecState == SS_RUNNING; }
    bool IsPaused() const { return m_SceneExecState == SS_PAUSED; }
    void SetSceneExecState(SceneExecState state) { m_SceneExecState = state; }

    template <typename... Components>
    auto GetAllEntitiesWith() {
        return m_Registry.view<Components...>();
    }

private:
    entt::registry m_Registry{};
    Ref<SceneGraph> m_SceneGraph{ nullptr };
    SceneExecState m_SceneExecState{ SS_PAUSED };

    uint32_t m_ViewportWidth  = 0;
    uint32_t m_ViewportHeight = 0;

    // Need to find better implementation
    std::unordered_map<CName, int> m_EntityNameMap;

private:
    template <typename T>
    void OnComponentAdded(Entity entity, T& component);

    CName GenerateUniqueName(const CName& baseName);

    friend class Entity;
};

}  // namespace ly::scene