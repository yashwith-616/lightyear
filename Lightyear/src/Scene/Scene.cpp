#include "Lightyear/Scene/Scene.h"
#include "Lightyear/Core/Application.h"
#include "Lightyear/Core/Window.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"
#include "Lightyear/Renderer/Camera/SceneCamera.h"
#include "Lightyear/Scene/Components/Components.h"
#include "Lightyear/Scene/Entity.h"

namespace ly::scene {

#pragma region Copy Component Template
template <typename... Component>
static void CopyComponent(entt::registry& dst,
                          entt::registry& src,
                          const std::unordered_map<UUID, entt::entity>& enttMap) {
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
                          const std::unordered_map<UUID, entt::entity>& enttMap) {
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

#pragma region Entity Management
Entity Scene::CreateEntity(const CName& name) {
    return CreateEntity(UUID(), name);
}

Entity Scene::CreateEntity(const CName& name, const Entity& parent) {
    return CreateEntity(UUID(), name, std::cref(parent));
}

Entity Scene::CreateChildEntity(Entity parent, const CName& name) {
    Entity entity = CreateEntity(name, parent);
    AddChildNode(entity, parent);
    return entity;
}

/**
 * @brief Create a child entity to parent entity
 * This follows a circular doubly linked list setup.
 * 1. If 0 child, then prev and next == child.
 * 2. If more child,
 *    - lastChild = traverse totalChildren - 1
 *    - ChildEntity->prev = lastChild
 *    - ChildEntity->next = lastChild->next
 *    - lastChild->next = ChildEntity
 *    - FirstChildNode->prev = Entity;
 *
 * TODO: Ordering child node is not important, can use the second element to insert instead of
 * revolving around.
 *
 * @param parent the parent node to which this child is added
 * @param name the name of the parent node
 */
void Scene::AddChildNode(Entity childEntity, Entity newParent) {
    LY_CORE_ASSERT(m_Registry.valid(childEntity), "Child Entity is invalid");
    LY_CORE_ASSERT(m_Registry.valid(newParent), "New Parent Entity is invalid");

    auto& childRelation  = childEntity.GetComponent<RelationshipComponent>();
    auto& parentRelation = newParent.GetComponent<RelationshipComponent>();

    if (parentRelation.ChildrenCount == 0) {
        parentRelation.FirstChild = childEntity;
        return;
    }

    // When multiple child exists
    auto& fistChildRelation = m_Registry.get<RelationshipComponent>(parentRelation.FirstChild);

    auto lastSibling = parentRelation.FirstChild;
    for (std::size_t i{}; i < parentRelation.ChildrenCount - 1; i++) {
        lastSibling = m_Registry.get<RelationshipComponent>(lastSibling).NextSibling;
    }

    auto& lastSiblingRelation = m_Registry.get<RelationshipComponent>(lastSibling);
    childRelation.PrevSibling = lastSibling;
    childRelation.NextSibling = lastSiblingRelation.NextSibling;

    lastSiblingRelation.NextSibling = childEntity;
    fistChildRelation.PrevSibling   = childEntity;

    parentRelation.ChildrenCount++;
    return;
}

/**
 * @brief Remove child node from the parent and reattach the circular doubly linked list
 *
 * @param childEntity
 * @param parent
 */
void Scene::RemoveChildNode(Entity childEntity, Entity parent) {
    LY_CORE_ASSERT(m_Registry.valid(childEntity), "Child Entity is invalid");
    LY_CORE_ASSERT(m_Registry.valid(parent), "Parent Entity is invalid");

    auto& childRelation  = m_Registry.get<RelationshipComponent>(childEntity);
    auto& parentRelation = m_Registry.get<RelationshipComponent>(parent);

    LY_CORE_ASSERT(childRelation.Parent != parent, "Child Entity is not related to parent");

    // If multiple child
    if (parentRelation.ChildrenCount != 1) {
        auto& prevRelation = m_Registry.get<RelationshipComponent>(childRelation.PrevSibling);
        auto& nextRelation = m_Registry.get<RelationshipComponent>(childRelation.NextSibling);

        prevRelation.NextSibling = childRelation.NextSibling;
        nextRelation.PrevSibling = childRelation.PrevSibling;
    }

    childRelation.Parent      = entt::null;
    childRelation.PrevSibling = entt::null;
    childRelation.NextSibling = entt::null;
    parentRelation.ChildrenCount--;
}

/**
 * @brief Traverse all the relationship component and delete them recursively
 * @param entity the entity that is supposed to be destroyed
 */
void Scene::DestroyEntity(Entity entity) {
    LY_CORE_ASSERT(m_Registry.valid(entity), "Invalid entity");

    if (!m_Registry.any_of<RelationshipComponent>(entity)) {
        m_Registry.destroy(entity);
        return;
    }

    auto& entityRelation = m_Registry.get<RelationshipComponent>(entity);

    auto curr = entityRelation.FirstChild;
    entt::entity prev{};
    for (std::size_t i{}; i < entityRelation.ChildrenCount; i++) {
        prev = curr;
        curr = m_Registry.get<RelationshipComponent>(curr).NextSibling;
        DestroyEntity(Entity(prev, this));
    }

    if (entityRelation.Parent != entt::null) {
        auto& parentRelation = m_Registry.get<RelationshipComponent>(entityRelation.Parent);
        RemoveChildNode(entity, Entity(entityRelation.Parent, this));
        parentRelation.ChildrenCount--;
    }

    m_Registry.destroy(entity);
}

Entity Scene::DuplicateEntity(Entity entity) {
    LY_CORE_ASSERT(m_Registry.valid(entity), "Entity is invalid");

    // Recursively need to duplicate child entities.

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

#pragma endregion

// TODO: Can move this entire section to new class called SceneRuntime. Doesn't fit here. Scene need
// to manage scene_registry alone and not what happens at runtime.
#pragma region Runtime, EditorRuntime
void Scene::OnRuntimeStart() {}

void Scene::OnRuntimeStop() {}

void Scene::OnSimulationStart() {}

void Scene::OnSimulationEnd() {}

void Scene::OnUpdateRuntime(ly::Timestep deltaTime) {
    LY_CORE_ASSERT(IsRunning(), "EditorUpdate is performed when scene is not paused!");

    m_SceneData.Time = Application::Get().GetWindow().GetTime();

    CameraComponent cameraComp = GetPrimaryCameraEntity().GetComponent<CameraComponent>();
    renderer::Renderer::BeginScene(cameraComp.Camera, m_SceneData);

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
    m_SceneData.Time = Application::Get().GetWindow().GetTime();

    LY_CORE_ASSERT(IsPaused(), "EditorUpdate is performed when scene is not paused!");
    renderer::Renderer::BeginScene(std::static_pointer_cast<renderer::Camera>(camera), m_SceneData);

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

#pragma endregion

void Scene::OnViewportResize(uint32_t width, uint32_t height) {
    m_ViewportHeight = height;
    m_ViewportWidth  = width;

    GetPrimaryCameraEntity().GetComponent<CameraComponent>().Camera->Resize(width, height);
}

Entity Scene::CreateEntity(UUID UUID,
                           const CName& name,
                           std::optional<std::reference_wrapper<const Entity>> parent) {
    Entity entity{ m_Registry.create(), this };
    entity.AddComponent<IDComponent>(UUID);
    entity.AddComponent<TagComponent>(GenerateUniqueName(name));
    entity.AddComponent<MobilityComponent>(EMobilityType::STATIC);
    entity.AddComponent<TransformComponent>();

    if (!parent.has_value()) {
        entity.AddComponent<RelationshipComponent>();
    } else {
        entity.AddComponent<RelationshipComponent>(parent.value().get(), entity, entity);
    }

    m_EntityNameMap[entity.GetComponent<TagComponent>().Tag] = entity;
    return entity;
}

// TODO: NEED TO UPDATE THE LOGIC OF GETTING NAMES. DO NOT LIKE THE CURRENT IMPLEMENTATION
/**
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

template <>
void Scene::OnComponentAdded<RelationshipComponent>(Entity entity,
                                                    RelationshipComponent& component) {}

#pragma endregion

}  // namespace ly::scene