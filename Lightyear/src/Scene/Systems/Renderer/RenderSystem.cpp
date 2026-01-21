#include "Lightyear/Scene/Systems/Renderer/RenderSystem.h"

#include "Lightyear/Renderer/Abstract/Renderer.h"
#include "Lightyear/Scene/Components/Core/DirtyComponent.h"
#include "Lightyear/Scene/Components/Render/MeshComponent.h"
#include "Lightyear/Scene/Components/Render/RenderPropertyComponent.h"
#include "Lightyear/Scene/Components/Transform/TransformComponent.h"

namespace ly::scene
{
RenderSystem::RenderSystem() : ISystem("Renderer", SystemLayer::Gameplay) {}

void RenderSystem::init(entt::registry& registry)
{
    // Create a scene tree
}

void RenderSystem::execute(entt::registry& registry)
{
    auto view = registry.view<TransformComponent, MeshComponent, RenderComponent, DirtyComponent>();
    for (auto&& [entity, transform, mesh, renderProperty, dirty] : view.each())
    {
        if (!mesh.shaderAsset || !mesh.meshAsset)
        {
            LY_CORE_LOG(LogType::Warn, "Submission skipped");
            continue;
        }

        auto submission =
            renderer::RenderSubmission(mesh.shaderAsset, mesh.meshAsset, mesh.textureAsset, transform.getTransform());

        renderer::Renderer::submit(submission);
    }
}

void RenderSystem::shutdown(entt::registry& registry)
{
    // Destroy the scene tree
}

} // namespace ly::scene
