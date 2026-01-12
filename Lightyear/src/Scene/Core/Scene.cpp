#include "Lightyear/Scene/Core/Scene.h"
#include "Lightyear/Scene/Components/Camera/CameraTag.h"
#include "Lightyear/Scene/Components/Components.h"
#include "Lightyear/Scene/Core/Entity.h"

// NOLINTBEGIN
namespace {
template <typename... Component>
void CopyComponent(entt::registry& dst,
                   entt::registry& src,
                   const std::unordered_map<ly::UUID, entt::entity>& enttMap) {
    (
        [&]() {
            auto view = src.view<Component>();
            for (auto srcEntity : view) {
                entt::entity dstEntity = enttMap.at(src.get<ly::scene::IDComponent>(srcEntity).ID);

                auto& srcComponent = src.get<Component>(srcEntity);
                dst.emplace_or_replace<Component>(dstEntity, srcComponent);
            }
        }(),
        ...);
}

template <typename... Component>
void CopyComponent(ly::scene::ComponentGroup<Component...>,
                   entt::registry& dst,
                   entt::registry& src,
                   const std::unordered_map<ly::UUID, entt::entity>& enttMap) {
    CopyComponent<Component...>(dst, src, enttMap);
}

template <typename... Component>
void CopyComponentIfExists(ly::scene::Entity dst, ly::scene::Entity src) {
    (
        [&]() {
            if (src.HasComponent<Component>()) {
                dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
            }
        }(),
        ...);
}

template <typename... Component>
void CopyComponentIfExists(ly::scene::ComponentGroup<Component...>, ly::scene::Entity dst, ly::scene::Entity src) {
    CopyComponentIfExists<Component...>(dst, src);
}
}  // namespace
// NOLINTEND

namespace ly::scene {

Scene::Scene() {
    m_Registry.on_update<TransformComponent>().connect<&Scene::OnTransformUpdated>();
    m_Registry.on_update<CameraComponent>().connect<&Scene::OnCameraUpdated>();
}

#pragma region Entity Management
Entity Scene::CreateEntity(const std::string& name) {
    return CreateEntity(UUID(), name);
}

Entity Scene::CreateEntity(const std::string& name, const Entity& parent) {
    return CreateEntity(UUID(), name, std::cref(parent));
}

Entity Scene::CreateChildEntity(Entity parent, const std::string& name) {
    const Entity entity = CreateEntity(name, parent);
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
 * @param newParent the parent node to which this child is added
 * @param childEntity the name of the parent node
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

Entity Scene::FindEntityByName(const std::string& name) const {
    auto view = m_Registry.view<TagComponent>();
    for (const auto entity : view) {
        const TagComponent& tagComponent = view.get<TagComponent>(entity);
        if (tagComponent.Tag == name) {
            return Entity{ entity, const_cast<Scene*>(this) };
        }
    }

    return {};
}

/**
 * Primary camera is game camera in runtime. Else, it is the editor camera present in the scene.
 * @return The camera entity
 */
Entity Scene::GetPrimaryCameraEntity() const {
    auto view = m_Registry.view<MainCameraTag>();
    for (auto entity : view) {
        return Entity{ entity, const_cast<Scene*>(this) };
    }
    return {};
}

#pragma endregion

Entity Scene::CreateEntity(UUID UUID,
                           const std::string& name,
                           std::optional<std::reference_wrapper<const Entity>> parent) {
    Entity entity{ m_Registry.create(), this };
    entity.AddComponent<IDComponent>(UUID);
    entity.AddComponent<TagComponent>(GenerateUniqueName(name));
    entity.AddComponent<TransformComponent>();
    entity.AddComponent<DirtyComponent>();

    if (!parent.has_value()) {
        entity.AddComponent<RelationshipComponent>();
    } else {
        entity.AddComponent<RelationshipComponent>(parent.value().get(), entity, entity);
    }

    m_EntityNameMap[entity.GetComponent<TagComponent>().Tag] = static_cast<uint32_t>(entity);
    return entity;
}

// TODO: NEED TO UPDATE THE LOGIC OF GETTING NAMES. DO NOT LIKE THE CURRENT IMPLEMENTATION
/**
 * Cons:
 * 1. Ability to rename has been lost. Need to update this again.
 * 2. Can attach the editorial names to the SceneNode. Makes it heavy and may cause issue when
 * converting SceneGraph to SceneTree.
 */
std::string Scene::GenerateUniqueName(const std::string& baseName) {
    auto& count = m_EntityNameMap[baseName];
    if (count++ > 0) {
        return std::format("{}{}", baseName, count);
    }
    return baseName;
}

void Scene::OnTransformUpdated(entt::registry& registry, entt::entity entity) {
    LY_CORE_ASSERT(registry.any_of<DirtyComponent>(entity), "Transform does not have a dirty component");

    auto& dirty     = registry.get<DirtyComponent>(entity);
    dirty.Transform = true;
}

void Scene::OnCameraUpdated(entt::registry& registry, entt::entity entity) {
    LY_CORE_ASSERT(registry.any_of<DirtyComponent>(entity), "Camera does not have a dirty component");

    auto& dirty             = registry.get<DirtyComponent>(entity);
    dirty.Camera_View       = true;
    dirty.Camera_Projection = true;
}

}  // namespace ly::scene