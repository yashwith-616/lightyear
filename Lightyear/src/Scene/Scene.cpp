#include "Lightyear/Scene/Scene.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"
#include "Lightyear/Renderer/Camera/SceneCamera.h"
#include "Lightyear/Scene/Components.h"
#include "Lightyear/Scene/Entity.h"

namespace ly::scene {

#pragma region Copy Component Template
template <typename... Component>
static void CopyComponent(entt::registry& dst,
                          entt::registry& src,
                          const std::unordered_map<uuid, entt::entity>& enttMap) {
    (
        [&]() {
            auto view = src.view<Component>();
            for (auto srcEntity : view) {
                entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity).ID);

                auto& srcComponent = src.get<Component>(srcEntity);
                dst.emplace_or_replace<Component>(dstEntity, srcComponent);
            }
        }(),
        ...);
}

template <typename... Component>
static void CopyComponent(ComponentGroup<Component...>,
                          entt::registry& dst,
                          entt::registry& src,
                          const std::unordered_map<uuid, entt::entity>& enttMap) {
    CopyComponent<Component...>(dst, src, enttMap);
}

template <typename... Component>
static void CopyComponentIfExists(Entity dst, Entity src) {
    (
        [&]() {
            if (src.HasComponent<Component>())
                dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
        }(),
        ...);
}

template <typename... Component>
static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src) {
    CopyComponentIfExists<Component...>(dst, src);
}

#pragma endregion

Entity Scene::CreateEntity(const CName& name) {
    return CreateEntity(uuid(), name);
}

/**
 * @brief The following entities are present in the scene or the world.
 * @param uuid The unique identifier for the entity
 * @param name The editor identifier name
 * @return The created entity
 */
Entity Scene::CreateEntity(uuid uuid, const CName& name) {
    Entity entity{ m_Registry.create(), this };
    entity.AddComponent<IDComponent>(uuid);
    entity.AddComponent<TagComponent>(GenerateUniqueName(name));
    entity.AddComponent<MobilityComponent>(EMobilityType::STATIC);
    entity.AddComponent<TransformComponent>();

    m_EntityNameMap[entity.GetComponent<TagComponent>().Tag] = entity;
    return entity;
}

void Scene::DestroyEntity(Entity entity) {
    m_Registry.destroy(entity);
}

Entity Scene::DuplicateEntity(Entity entity) {
    const std::string name = entity.GetName();
    Entity newEntity       = CreateEntity(name);
    CopyComponentIfExists(AllComponents{}, newEntity, entity);
    return newEntity;
}

Entity Scene::FindEntityByName(const CName& name) const {
    auto view = m_Registry.view<TagComponent>();
    for (auto entity : view) {
        const TagComponent& tc = view.get<TagComponent>(entity);
        if (tc.Tag == name) {
            return Entity{ entity, const_cast<Scene*>(this) };
        }
    }

    return {};
}

Entity Scene::GetPrimaryCameraEntity() const {
    auto view = m_Registry.view<CameraComponent>();
    for (auto entity : view) {
        const auto& camera = view.get<CameraComponent>(entity);
        if (camera.bIsPrimary) {
            return Entity{ entity, const_cast<Scene*>(this) };
        }
    }
    return {};
}

void Scene::OnRuntimeStart() {}

void Scene::OnRuntimeStop() {}

void Scene::OnSimulationStart() {}

void Scene::OnSimulationEnd() {}

void Scene::OnUpdateRuntime(ly::Timestep deltaTime) {
    LY_CORE_ASSERT(IsRunning(), "EditorUpdate is performed when scene is not paused!");

    CameraComponent cameraComp = GetPrimaryCameraEntity().GetComponent<CameraComponent>();
    renderer::Renderer::BeginScene(cameraComp.Camera);

    auto view = m_Registry.view<RenderComponent, MeshComponent, TransformComponent>();
    for (auto entity : view) {
        const auto& [render, mesh, transform] =
            view.get<RenderComponent, MeshComponent, TransformComponent>(entity);

        if (!mesh.ShaderAsset || !mesh.MeshAsset) {
            continue;
        }

        renderer::Renderer::Submit(renderer::RenderSubmission(
            mesh.ShaderAsset, mesh.MeshAsset, mesh.TextureAsset, transform.GetTransform()));
    }

    renderer::Renderer::EndScene();
}

void Scene::OnUpdateSimulation(ly::Timestep deltaTime, Ref<renderer::SceneCamera> camera) {
    // Update Physics
}

void Scene::OnUpdateEditor(ly::Timestep deltaTime, Ref<renderer::SceneCamera> camera) {
    LY_CORE_ASSERT(IsPaused(), "EditorUpdate is performed when scene is not paused!");
    renderer::Renderer::BeginScene(std::static_pointer_cast<renderer::Camera>(camera));

    auto view = m_Registry.view<RenderComponent, MeshComponent, TransformComponent>();
    for (auto entity : view) {
        const auto& [render, mesh, transform] =
            view.get<RenderComponent, MeshComponent, TransformComponent>(entity);

        if (!mesh.ShaderAsset || !mesh.MeshAsset) {
            continue;
        }

        renderer::Renderer::Submit(renderer::RenderSubmission(
            mesh.ShaderAsset, mesh.MeshAsset, mesh.TextureAsset, transform.GetTransform()));
    }

    renderer::Renderer::EndScene();
}

void Scene::OnViewportResize(uint32_t width, uint32_t height) {
    m_ViewportHeight = height;
    m_ViewportWidth  = width;

    GetPrimaryCameraEntity().GetComponent<CameraComponent>().Camera->Resize(width, height);
}

/**
 * @brief NEED TO UPDATE THE LOGIC OF GETTING NAMES. DO NOT LIKE THE CURRENT IMPLEMENTATION
 * Cons:
 * 1. Ability to rename has been lost. Need to update this again.
 * 2. Can attach the editorial names to the ScneNode. Makes it heavy and may cause issue when
 * converting SceneGraph to SceneTree.
 */
CName Scene::GenerateUniqueName(const CName& baseName) {
    auto& count = m_EntityNameMap[baseName];
    if (count++ > 0) {
        return std::format("{}{}", baseName, count);
    }
    return baseName;
}

template <typename T>
void Scene::OnComponentAdded(Entity entity, T& component) {
    static_assert(sizeof(T) == 0);
}

#pragma region Component Template Specialization
template <>
void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component) {}

template <>
void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {}

template <>
void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component) {
    if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
        component.Camera->Resize(m_ViewportWidth, m_ViewportHeight);
}

template <>
void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component) {}

template <>
void Scene::OnComponentAdded<MobilityComponent>(Entity entity, MobilityComponent& component) {}

template <>
void Scene::OnComponentAdded<MeshComponent>(Entity entity, MeshComponent& component) {}

template <>
void Scene::OnComponentAdded<RenderComponent>(Entity entity, RenderComponent& component) {}

#pragma endregion

}  // namespace ly::scene