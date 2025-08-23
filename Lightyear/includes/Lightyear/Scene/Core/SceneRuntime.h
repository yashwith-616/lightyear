#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/SceneData.h"
#include "Lightyear/Scene/Core/Scene.h"

namespace ly::renderer {
class SceneCamera;
}

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
     * @param editorCamera the editor camera that is part of the scene
     */
    void OnUpdateSimulation(Timestep deltaTime, Ref<renderer::SceneCamera> editorCamera);

    /**
     * @brief The following update is performed on the editor while rendering using the sceneCamera
     * @param deltaTime the time between each frame
     * @param camera the editorial camera
     */
    void OnUpdateEditor(Timestep deltaTime, const Ref<renderer::SceneCamera>& camera);

    void OnViewportResize(glm::uvec2 size);

    void SetSceneExecState(SceneExecState state) { m_SceneExecState = state; }
    bool IsRunning() const { return m_SceneExecState == SceneExecState::RUNNING; }
    bool IsPaused() const { return m_SceneExecState == SceneExecState::PAUSED; }

private:
    Scene* m_WPtrScene{};
    glm::uvec2 m_ViewportSize{};
    SceneExecState m_SceneExecState{ SceneExecState::PAUSED };
    SceneData m_SceneData{};
};

}  // namespace ly::scene