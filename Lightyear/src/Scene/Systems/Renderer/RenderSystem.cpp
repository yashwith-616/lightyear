#include "Lightyear/Scene/Systems/Renderer/RenderSystem.h"

#include "Lightyear/Renderer/Abstract/Renderer.h"
#include "Lightyear/Scene/Components/Core/DirtyComponent.h"
#include "Lightyear/Scene/Components/Render/MeshComponent.h"
#include "Lightyear/Scene/Components/Render/RenderPropertyComponent.h"
#include "Lightyear/Scene/Components/Transform/TransformComponent.h"

namespace ly::scene {
RenderSystem::RenderSystem() : ISystem("Renderer", SystemLayer::Gameplay) {}

void RenderSystem::Init(entt::registry& registry) {
    // Create a scene tree
}

void RenderSystem::Execute(entt::registry& registry) {
    auto view = registry.view<TransformComponent, MeshComponent, RenderComponent, DirtyComponent>();
    for (auto&& [entity, transform, mesh, renderProperty, dirty] : view.each()) {
        if (!mesh.ShaderAsset || !mesh.MeshAsset) {
            continue;
        }

        auto submission =
            renderer::RenderSubmission(mesh.ShaderAsset, mesh.MeshAsset, mesh.TextureAsset, transform.GetTransform());

        renderer::Renderer::Submit(submission);
    }
}

void RenderSystem::Shutdown(entt::registry& registry) {
    // Destroy the scene tree
}

}  // namespace ly::scene