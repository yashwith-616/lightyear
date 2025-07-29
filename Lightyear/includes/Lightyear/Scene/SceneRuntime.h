#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Scene.h"
#include "Lightyear/Scene/SceneData.h"
#include "Lightyear/Scene/Systems/ISystem.h"

namespace ly::scene {

enum class SceneExecState : uint8_t { NONE = 0, PAUSED = 1, RUNNING = 2, SIMULATION = 3, MAX = 4 };

class LIGHTYEAR_API SceneRuntime {
public:
    explicit SceneRuntime(Scene* scene);
    SceneRuntime(const SceneRuntime&)            = delete;
    SceneRuntime& operator=(const SceneRuntime&) = delete;
    SceneRuntime(SceneRuntime&&)                 = default;
    SceneRuntime& operator=(SceneRuntime&&)      = default;

    ~SceneRuntime() = default;

    void Initialize();

    void OnRuntimeStart();
    void OnRuntimeStop();

    void OnSimulationStart();
    void OnSimulationStop();

    /**
     * @brief The following update is performed on the editor along with games logical layer and
     * using the main camera that is part of the game
     * @param deltaTime the timestep between each frame
     */
    void OnUpdateRuntime(Timestep deltaTime);

    /**
     * @brief Update physics layer and render using sceneCamera
     *
     * @param deltaTime the time between each frame
     */
    void OnUpdateSimulation(Timestep deltaTime);

    /**
     * @brief The following update is performed on the editor while rendering using the sceneCamera
     * @param deltaTime the time between each frame
     */
    void OnUpdateEditor(Timestep deltaTime);

    void OnViewportResize(glm::uvec2 size);

    void SetSceneExecState(SceneExecState state) { m_SceneExecState = state; }
    [[nodiscard]] bool IsRunning() const { return m_SceneExecState == SceneExecState::RUNNING; }
    [[nodiscard]] bool IsPaused() const { return m_SceneExecState == SceneExecState::PAUSED; }

private:
    Scene* m_WPtrScene{};
    glm::uvec2 m_ViewportSize{};
    SceneExecState m_SceneExecState{ SceneExecState::PAUSED };
    SceneData m_SceneData{};

    std::vector<Scope<ISystem>> m_SceneSystems;
};

}  // namespace ly::scene