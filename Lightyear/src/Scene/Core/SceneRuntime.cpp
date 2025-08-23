#include "Lightyear/Scene/Core/SceneRuntime.h"

#include "Lightyear/Core/Application.h"
#include "Lightyear/Core/Window.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"
#include "Lightyear/Renderer/Camera/SceneCamera.h"
#include "Lightyear/Scene/Components/Components.h"
#include "Lightyear/Scene/Core/Entity.h"

namespace ly::scene {

SceneRuntime::SceneRuntime(Scene* scene) : m_WPtrScene(scene) {}

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

    const auto cameraComp = m_WPtrScene->GetPrimaryCameraEntity().GetComponent<CameraComponent>();
    renderer::Renderer::BeginScene(cameraComp.Camera, m_SceneData);

    const auto& registry = m_WPtrScene->GetRegistry();
    auto view            = registry.view<RenderComponent, MeshComponent, TransformComponent>();
    for (auto entity : view) {
        const auto& [render, mesh, transform] = view.get<RenderComponent, MeshComponent, TransformComponent>(entity);

        if (!mesh.ShaderAsset || !mesh.MeshAsset) {
            continue;
        }

        renderer::Renderer::Submit(
            renderer::RenderSubmission(mesh.ShaderAsset, mesh.MeshAsset, mesh.TextureAsset, transform.GetTransform()));
    }

    renderer::Renderer::EndScene();
}

// NOLINTNEXTLINE
void SceneRuntime::OnUpdateSimulation(Timestep /*deltaTime*/, Ref<renderer::SceneCamera> /*camera*/) {
    // TODO: Update physics simulation logic only
    LY_CORE_ASSERT(false, "OnUpdateSimulation has not been updated yet");
}

void SceneRuntime::OnUpdateEditor(Timestep /*deltaTime*/, const Ref<renderer::SceneCamera>& camera) {
    m_SceneData.Time = Application::Get().GetWindow().GetTime();

    LY_CORE_ASSERT(IsPaused(), "EditorUpdate is performed when scene is not paused!");
    renderer::Renderer::BeginScene(std::static_pointer_cast<renderer::Camera>(camera), m_SceneData);

    const auto& registry = m_WPtrScene->GetRegistry();
    auto view            = registry.view<RenderComponent, MeshComponent, TransformComponent>();
    for (auto entity : view) {
        const auto& [render, mesh, transform] = view.get<RenderComponent, MeshComponent, TransformComponent>(entity);

        if (!mesh.ShaderAsset || !mesh.MeshAsset) {
            continue;
        }

        renderer::Renderer::Submit(
            renderer::RenderSubmission(mesh.ShaderAsset, mesh.MeshAsset, mesh.TextureAsset, transform.GetTransform()));
    }

    renderer::Renderer::EndScene();
}

void SceneRuntime::OnViewportResize(glm::uvec2 size) {
    m_ViewportSize.x = size.x;
    m_ViewportSize.y = size.y;

    // NOLINTNEXTLINE
    m_WPtrScene->GetPrimaryCameraEntity().GetComponent<CameraComponent>().Camera->Resize(size.x, size.y);
}

}  // namespace ly::scene