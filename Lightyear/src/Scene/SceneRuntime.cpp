#include "Lightyear/Scene/SceneRuntime.h"

#include <Lightyear/Scene/Components/Core/DirtyComponent.h>

#include "Lightyear/Core/Application.h"
#include "Lightyear/Core/Window.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

#include "Lightyear/Scene/Components/Components.h"
#include "Lightyear/Scene/Entity.h"
#include "Lightyear/Scene/Systems/Camera/CameraSystem.h"
#include "Lightyear/Scene/Systems/Renderer/RenderSystem.h"

namespace ly::scene {

SceneRuntime::SceneRuntime(Scene* scene) : m_WPtrScene(scene) {}

void SceneRuntime::Initialize() {
    m_SceneSystems.push_back(MakeScope<CameraSystem>());
    m_SceneSystems.push_back(MakeScope<RenderSystem>());
}

// NOLINTNEXTLINE
void SceneRuntime::OnRuntimeStart() {
    LY_LOG(ly::LogType::Warn, "OnRuntimeStart has not been updated yet");
}

// NOLINTNEXTLINE
void SceneRuntime::OnRuntimeStop() {
    LY_CORE_ASSERT(false, "OnRuntimeStop has not been updated yet");
}

// NOLINTNEXTLINE
void SceneRuntime::OnSimulationStart() {
    LY_CORE_ASSERT(false, "OnSimulationStart has not been updated yet");
}

// NOLINTNEXTLINE
void SceneRuntime::OnSimulationStop() {
    LY_CORE_ASSERT(false, "OnSimulationStop has not been updated yet");
}

void SceneRuntime::OnUpdateRuntime(Timestep /*deltaTime*/) {
    LY_CORE_ASSERT(IsRunning(), "EditorUpdate is performed when scene is not paused!");

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
void SceneRuntime::OnUpdateSimulation(Timestep /*deltaTime*/) {
    // TODO: Update physics simulation logic only
    LY_CORE_ASSERT(false, "OnUpdateSimulation has not been updated yet");
}

void SceneRuntime::OnUpdateEditor(Timestep /*deltaTime*/) {
    m_SceneData.Time = Application::Get().GetWindow().GetTime();
    LY_CORE_ASSERT(IsPaused(), "EditorUpdate is performed when scene is not paused!");

    renderer::Renderer::EndScene();
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

}  // namespace ly::scene