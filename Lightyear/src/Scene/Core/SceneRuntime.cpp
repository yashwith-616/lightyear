#include "Lightyear/Scene/Core/SceneRuntime.h"

#include "Lightyear/Core/Application.h"
#include "Lightyear/Core/Window.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"
#include "Lightyear/Scene/Components/Components.h"
#include "Lightyear/Scene/Components/Core/DirtyComponent.h"
#include "Lightyear/Scene/Core/Entity.h"
#include "Lightyear/Scene/Systems/Camera/CameraSystem.h"
#include "Lightyear/Scene/Systems/Renderer/RenderSystem.h"

namespace ly::scene {

SceneRuntime::SceneRuntime(Scene* scene) : m_wPtrScene(scene) {}

void SceneRuntime::initialize() {
    // Initialize Systems
    m_sceneSystems.push_back(makeScope<CameraSystem>());
    m_sceneSystems.push_back(makeScope<RenderSystem>());
}

void SceneRuntime::onBegin() {
    switch (m_sceneMode) {
        case SceneRuntimeMode::Pause:
            onEditBegin();
            break;
        case SceneRuntimeMode::Play:
            onRuntimeBegin();
            break;
        case SceneRuntimeMode::Simulation:
            onSimulationBegin();
            break;
        default:
            LY_CORE_ASSERT(false, "Invalid scene runtime mode {}", static_cast<int>(m_sceneMode));
    }
}

void SceneRuntime::onUpdate(float deltaTime) {
    switch (m_sceneMode) {
        case SceneRuntimeMode::Pause:
            onEditUpdate(deltaTime);
            break;
        case SceneRuntimeMode::Play:
            onRuntimeUpdate(deltaTime);
            break;
        case SceneRuntimeMode::Simulation:
            onSimulationUpdate(deltaTime);
            break;
        default:
            LY_CORE_ASSERT(false, "Invalid scene runtime mode {}", static_cast<int>(m_sceneMode));
    }
}

void SceneRuntime::onEnd() {
    switch (m_sceneMode) {
        case SceneRuntimeMode::Pause:
            onEditEnd();
            break;
        case SceneRuntimeMode::Play:
            onRuntimeEnd();
            break;
        case SceneRuntimeMode::Simulation:
            onSimulationEnd();
            break;
        default:
            LY_CORE_ASSERT(false, "Invalid scene runtime mode {}", static_cast<int>(m_sceneMode));
    }
}

void SceneRuntime::onViewportResize(glm::uvec2 size) {
    m_viewportSize.x = size.x;
    m_viewportSize.y = size.y;

    float const kNewAspectRatio = static_cast<float>(size.x) / static_cast<float>(size.y);

    Entity primaryCamera   = m_wPtrScene->getPrimaryCameraEntity();
    auto& cameraComp       = primaryCamera.getComponent<CameraComponent>();
    cameraComp.aspectRatio = kNewAspectRatio;

    auto& dirty            = primaryCamera.getComponent<DirtyComponent>();
    dirty.cameraProjection = true;
}

#pragma region PlayMode
// NOLINTNEXTLINE
void SceneRuntime::onRuntimeBegin() {
    LY_CORE_ASSERT(false, "OnRuntimeBegin has not been updated yet");
}

void SceneRuntime::onRuntimeUpdate(Timestep /*deltaTime*/) {
    LY_CORE_ASSERT(isRunning(), "OnUpdate is performed when scene is not paused!");
    m_sceneData.time = Application::get().getWindow().getTime();

    Entity mainCamera          = m_wPtrScene->getPrimaryCameraEntity();
    auto const cameraComp      = mainCamera.getComponent<CameraComponent>();
    auto const cameraTransform = mainCamera.getComponent<TransformComponent>();
    renderer::Renderer::beginScene(cameraComp, cameraTransform, m_sceneData);

    auto& registry = m_wPtrScene->getRegistry();
    for (auto const& system : m_sceneSystems) {
        system->execute(registry);
    }

    renderer::Renderer::endScene();
}

// NOLINTNEXTLINE
void SceneRuntime::onRuntimeEnd() {
    LY_CORE_ASSERT(false, "OnRuntimeEnd has not been updated yet");
}

#pragma endregion

#pragma region SimulateMode
// NOLINTNEXTLINE
void SceneRuntime::onSimulationBegin() {
    LY_CORE_ASSERT(false, "OnSimulationStart has not been updated yet");
}

// NOLINTNEXTLINE
void SceneRuntime::onSimulationUpdate(Timestep /*deltaTime*/) {
    LY_CORE_ASSERT(false, "OnUpdateSimulation has not been updated yet");
}

// NOLINTNEXTLINE
void SceneRuntime::onSimulationEnd() {
    LY_CORE_ASSERT(false, "OnSimulationStop has not been updated yet");
}
#pragma endregion

#pragma region EditMode

// NOLINTNEXTLINE
void SceneRuntime::onEditBegin() {}

// NOLINTNEXTLINE
void SceneRuntime::onEditUpdate(Timestep deltaTime) {}

// NOLINTNEXTLINE
void SceneRuntime::onEditEnd() {}

#pragma endregion

}  // namespace ly::scene