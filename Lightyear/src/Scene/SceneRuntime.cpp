#include "Lightyear/Scene/SceneRuntime.h"

#include "Lightyear/Core/Application.h"
#include "Lightyear/Core/Window.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"
#include "Lightyear/Scene/Components/Components.h"
#include "Lightyear/Scene/Components/Core/DirtyComponent.h"
#include "Lightyear/Scene/Entity.h"
#include "Lightyear/Scene/Systems/Camera/CameraSystem.h"
#include "Lightyear/Scene/Systems/Renderer/RenderSystem.h"

namespace ly::scene {

SceneRuntime::SceneRuntime(Scene* scene) : m_WPtrScene(scene) {}

void SceneRuntime::Initialize() {
    // Initialize Systems
    m_SceneSystems.push_back(MakeScope<CameraSystem>());
    m_SceneSystems.push_back(MakeScope<RenderSystem>());
}

void SceneRuntime::OnBegin() {
    switch (m_SceneMode) {
        case SceneRuntimeMode::PAUSE:
            OnEditBegin();
            break;
        case SceneRuntimeMode::PLAY:
            OnRuntimeBegin();
            break;
        case SceneRuntimeMode::SIMULATION:
            OnSimulationBegin();
            break;
        default:
            LY_CORE_ASSERT(false, "Invalid scene runtime mode {}", static_cast<int>(m_SceneMode));
    }
}

void SceneRuntime::OnUpdate(float deltaTime) {
    switch (m_SceneMode) {
        case SceneRuntimeMode::PAUSE:
            OnEditUpdate(deltaTime);
            break;
        case SceneRuntimeMode::PLAY:
            OnRuntimeUpdate(deltaTime);
            break;
        case SceneRuntimeMode::SIMULATION:
            OnSimulationUpdate(deltaTime);
            break;
        default:
            LY_CORE_ASSERT(false, "Invalid scene runtime mode {}", static_cast<int>(m_SceneMode));
    }
}

void SceneRuntime::OnEnd() {
    switch (m_SceneMode) {
        case SceneRuntimeMode::PAUSE:
            OnEditEnd();
            break;
        case SceneRuntimeMode::PLAY:
            OnRuntimeEnd();
            break;
        case SceneRuntimeMode::SIMULATION:
            OnSimulationEnd();
            break;
        default:
            LY_CORE_ASSERT(false, "Invalid scene runtime mode {}", static_cast<int>(m_SceneMode));
    }
}

void SceneRuntime::OnViewportResize(glm::uvec2 size) {
    m_ViewportSize.x = size.x;
    m_ViewportSize.y = size.y;

    const float kNewAspectRatio = static_cast<float>(size.x) / static_cast<float>(size.y);

    Entity primaryCamera   = m_WPtrScene->GetPrimaryCameraEntity();
    auto& cameraComp       = primaryCamera.GetComponent<CameraComponent>();
    cameraComp.AspectRatio = kNewAspectRatio;

    auto& dirty             = primaryCamera.GetComponent<DirtyComponent>();
    dirty.Camera_Projection = true;
}

#pragma region PlayMode
// NOLINTNEXTLINE
void SceneRuntime::OnRuntimeBegin() {
    LY_CORE_ASSERT(false, "OnRuntimeBegin has not been updated yet");
}

void SceneRuntime::OnRuntimeUpdate(Timestep /*deltaTime*/) {
    LY_CORE_ASSERT(IsRunning(), "OnUpdate is performed when scene is not paused!");
    m_SceneData.Time = Application::Get().GetWindow().GetTime();

    Entity mainCamera          = m_WPtrScene->GetPrimaryCameraEntity();
    const auto cameraComp      = mainCamera.GetComponent<CameraComponent>();
    const auto cameraTransform = mainCamera.GetComponent<TransformComponent>();
    renderer::Renderer::BeginScene(cameraComp, cameraTransform, m_SceneData);

    auto& registry = m_WPtrScene->GetRegistry();
    for (const auto& system : m_SceneSystems) {
        system->Execute(registry);
    }

    renderer::Renderer::EndScene();
}

// NOLINTNEXTLINE
void SceneRuntime::OnRuntimeEnd() {
    LY_CORE_ASSERT(false, "OnRuntimeEnd has not been updated yet");
}

#pragma endregion

#pragma region SimulateMode
// NOLINTNEXTLINE
void SceneRuntime::OnSimulationBegin() {
    LY_CORE_ASSERT(false, "OnSimulationStart has not been updated yet");
}

// NOLINTNEXTLINE
void SceneRuntime::OnSimulationUpdate(Timestep /*deltaTime*/) {
    LY_CORE_ASSERT(false, "OnUpdateSimulation has not been updated yet");
}

// NOLINTNEXTLINE
void SceneRuntime::OnSimulationEnd() {
    LY_CORE_ASSERT(false, "OnSimulationStop has not been updated yet");
}
#pragma endregion

#pragma region EditMode

// NOLINTNEXTLINE
void SceneRuntime::OnEditBegin() {}

// NOLINTNEXTLINE
void SceneRuntime::OnEditUpdate(Timestep deltaTime) {}

// NOLINTNEXTLINE
void SceneRuntime::OnEditEnd() {}

#pragma endregion

}  // namespace ly::scene