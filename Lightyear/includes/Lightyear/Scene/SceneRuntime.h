#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Enums/SceneRuntimeMode.h"
#include "Lightyear/Scene/Scene.h"
#include "Lightyear/Scene/SceneData.h"
#include "Lightyear/Scene/Systems/ISystem.h"

namespace ly::scene {

class LIGHTYEAR_API SceneRuntime {
public:
    explicit SceneRuntime(Scene* scene);
    ~SceneRuntime() = default;

    SceneRuntime(const SceneRuntime&)            = delete;
    SceneRuntime& operator=(const SceneRuntime&) = delete;

    SceneRuntime(SceneRuntime&&)            = default;
    SceneRuntime& operator=(SceneRuntime&&) = default;

    void Initialize();
    void OnBegin();
    void OnUpdate(float deltaTime);
    void OnEnd();

    void OnViewportResize(glm::uvec2 size);

    void SetSceneExecState(SceneRuntimeMode state) { m_SceneMode = state; }
    [[nodiscard]] bool IsRunning() const { return m_SceneMode == SceneRuntimeMode::PLAY; }
    [[nodiscard]] bool IsPaused() const { return m_SceneMode == SceneRuntimeMode::PAUSE; }

private:
    void OnRuntimeBegin();
    void OnRuntimeUpdate(Timestep deltaTime);
    void OnRuntimeEnd();

    void OnSimulationBegin();
    void OnSimulationUpdate(Timestep deltaTime);
    void OnSimulationEnd();

    void OnEditBegin();
    void OnEditUpdate(Timestep deltaTime);
    void OnEditEnd();

    Scene* m_WPtrScene{};
    glm::uvec2 m_ViewportSize{};
    SceneRuntimeMode m_SceneMode{ SceneRuntimeMode::PAUSE };
    SceneData m_SceneData{};
    std::vector<Scope<ISystem>> m_SceneSystems;
};

}  // namespace ly::scene