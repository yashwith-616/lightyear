#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Core/Scene.h"
#include "Lightyear/Scene/Enums/SceneRuntimeMode.h"
#include "Lightyear/Scene/SceneData.h"
#include "Lightyear/Scene/Systems/ISystem.h"

namespace ly::renderer
{
class SceneCamera;
}

namespace ly::scene
{

class LIGHTYEAR_API SceneRuntime
{
public:
    explicit SceneRuntime(Scene* scene);
    ~SceneRuntime() = default;

    SceneRuntime(SceneRuntime const&) = delete;
    SceneRuntime& operator=(SceneRuntime const&) = delete;

    SceneRuntime(SceneRuntime&&) = default;
    SceneRuntime& operator=(SceneRuntime&&) = default;

    void initialize();
    void onBegin();
    void onUpdate(float deltaTime);
    void onEnd();

    void onViewportResize(glm::uvec2 size);

    void setSceneExecState(SceneRuntimeMode state) { m_sceneMode = state; }
    [[nodiscard]] bool isRunning() const { return m_sceneMode == SceneRuntimeMode::Play; }
    [[nodiscard]] bool isPaused() const { return m_sceneMode == SceneRuntimeMode::Pause; }

private:
    void onRuntimeBegin();
    void onRuntimeUpdate(Timestep deltaTime);
    void onRuntimeEnd();

    void onSimulationBegin();
    void onSimulationUpdate(Timestep deltaTime);
    void onSimulationEnd();

    void onEditBegin();
    void onEditUpdate(Timestep deltaTime);
    void onEditEnd();

    Scene* m_wPtrScene{};
    glm::uvec2 m_viewportSize{};
    SceneRuntimeMode m_sceneMode{SceneRuntimeMode::Pause};
    SceneData m_sceneData{};
    std::vector<scope<ISystem>> m_sceneSystems;
};

} // namespace ly::scene
