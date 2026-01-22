#include "Lightyear/Scene/Core/Scene.h"
#include "Lightyear/Scene/Components/Camera/CameraTag.h"
#include "Lightyear/Scene/Components/Components.h"
#include "Lightyear/Scene/Core/Entity.h"

// NOLINTBEGIN
namespace
{
template <typename... Component>
void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<ly::Uuid, entt::entity>& enttMap)
{
    (
        [&]() {
            auto view = src.view<Component>();
            for (auto srcEntity : view)
            {
                entt::entity dstEntity = enttMap.at(src.get<ly::scene::IdComponent>(srcEntity).ID);

                auto& srcComponent = src.get<Component>(srcEntity);
                dst.emplace_or_replace<Component>(dstEntity, srcComponent);
            }
        }(),
        ...);
}

template <typename... Component>
void CopyComponent(
    ly::scene::ComponentGroup<Component...>,
    entt::registry& dst,
    entt::registry& src,
    const std::unordered_map<ly::Uuid, entt::entity>& enttMap)
{
    CopyComponent<Component...>(dst, src, enttMap);
}

template <typename... Component>
void CopyComponentIfExists(ly::scene::Entity dst, ly::scene::Entity src)
{
    (
        [&]() {
            if (src.hasComponent<Component>())
            {
                dst.addOrReplaceComponent<Component>(src.getComponent<Component>());
            }
        }(),
        ...);
}

template <typename... Component>
void CopyComponentIfExists(ly::scene::ComponentGroup<Component...>, ly::scene::Entity dst, ly::scene::Entity src)
{
    CopyComponentIfExists<Component...>(dst, src);
}
} // namespace
// NOLINTEND

namespace ly::scene
{

Scene::Scene()
{
    m_registry.on_update<TransformComponent>().connect<&Scene::onTransformUpdated>();
    m_registry.on_update<CameraComponent>().connect<&Scene::onCameraUpdated>();
}

#pragma region Entity Management
Entity Scene::createEntity(std::string const& name) { return createEntity(Uuid(), name); }

Entity Scene::createEntity(std::string const& name, Entity const& parent)
{
    return createEntity(Uuid(), name, std::cref(parent));
}

/**
 * @brief Traverse all the relationship component and delete them recursively
 * @param entity the entity that is supposed to be destroyed
 */
void Scene::destroyEntity(Entity entity)
{
    LY_CORE_ASSERT(m_registry.valid(entity), "Invalid entity");

    if (!m_registry.any_of<RelationshipComponent>(entity))
    {
        m_registry.destroy(entity);
        return;
    }

    auto& entityRelation = m_registry.get<RelationshipComponent>(entity);

    auto curr = entityRelation.firstChild;
    entt::entity prev{};
    for (std::size_t i{}; i < entityRelation.childrenCount; i++)
    {
        prev = curr;
        curr = m_registry.get<RelationshipComponent>(curr).nextSibling;
        destroyEntity(Entity(prev, this));
    }

    if (entityRelation.parent != entt::null)
    {
        auto& parentRelation = m_registry.get<RelationshipComponent>(entityRelation.parent);
        removeChildNode(entity, Entity(entityRelation.parent, this));
        parentRelation.childrenCount--;
    }

    m_registry.destroy(entity);
}

Entity Scene::duplicateEntity(Entity entity)
{
    LY_CORE_ASSERT(m_registry.valid(entity), "Entity is invalid");

    // Recursively need to duplicate child entities.
    std::string const name = entity.getName();
    Entity newEntity = createEntity(name);
    CopyComponentIfExists(allComponents{}, newEntity, entity);
    return newEntity;
}

Entity Scene::findEntityByName(std::string const& name) const
{
    auto view = m_registry.view<TagComponent>();
    for (auto const entity : view)
    {
        TagComponent const& tagComponent = view.get<TagComponent>(entity);
        if (tagComponent.tag == name)
        {
            return Entity{entity, const_cast<Scene*>(this)};
        }
    }

    return {};
}

Entity Scene::createChildEntity(Entity parent, std::string const& name)
{
    Entity const entity = createEntity(name, parent);
    addChildNode(entity, parent);
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
void Scene::addChildNode(Entity childEntity, Entity newParent)
{
    LY_CORE_ASSERT(m_registry.valid(childEntity), "Child Entity is invalid");
    LY_CORE_ASSERT(m_registry.valid(newParent), "New Parent Entity is invalid");

    auto& childRelation = childEntity.getComponent<RelationshipComponent>();
    auto& parentRelation = newParent.getComponent<RelationshipComponent>();

    if (parentRelation.childrenCount == 0)
    {
        parentRelation.firstChild = childEntity;
        return;
    }

    // When multiple child exists
    auto& fistChildRelation = m_registry.get<RelationshipComponent>(parentRelation.firstChild);

    auto lastSibling = parentRelation.firstChild;
    for (std::size_t i{}; i < parentRelation.childrenCount - 1; i++)
    {
        lastSibling = m_registry.get<RelationshipComponent>(lastSibling).nextSibling;
    }

    auto& lastSiblingRelation = m_registry.get<RelationshipComponent>(lastSibling);
    childRelation.prevSibling = lastSibling;
    childRelation.nextSibling = lastSiblingRelation.nextSibling;

    lastSiblingRelation.nextSibling = childEntity;
    fistChildRelation.prevSibling = childEntity;

    parentRelation.childrenCount++;
}

/**
 * @brief Remove child node from the parent and reattach the circular doubly linked list
 *
 * @param childEntity
 * @param parent
 */
void Scene::removeChildNode(Entity childEntity, Entity parent)
{
    LY_CORE_ASSERT(m_registry.valid(childEntity), "Child Entity is invalid");
    LY_CORE_ASSERT(m_registry.valid(parent), "Parent Entity is invalid");

    auto& childRelation = m_registry.get<RelationshipComponent>(childEntity);
    auto& parentRelation = m_registry.get<RelationshipComponent>(parent);

    LY_CORE_ASSERT(childRelation.parent != parent, "Child Entity is not related to parent");

    // If multiple child
    if (parentRelation.childrenCount != 1)
    {
        auto& prevRelation = m_registry.get<RelationshipComponent>(childRelation.prevSibling);
        auto& nextRelation = m_registry.get<RelationshipComponent>(childRelation.nextSibling);

        prevRelation.nextSibling = childRelation.nextSibling;
        nextRelation.prevSibling = childRelation.prevSibling;
    }

    childRelation.parent = entt::null;
    childRelation.prevSibling = entt::null;
    childRelation.nextSibling = entt::null;
    parentRelation.childrenCount--;
}

/**
 * Primary camera is game camera in runtime. Else, it is the editor camera present in the scene.
 * @return The camera entity
 */
Entity Scene::getPrimaryCameraEntity() const
{
    auto view = m_registry.view<MainCameraTag>();
    for (auto entity : view)
    {
        return Entity{entity, const_cast<Scene*>(this)};
    }
    return {};
}

#pragma endregion

Entity
    Scene::createEntity(Uuid uuid, std::string const& name, std::optional<std::reference_wrapper<Entity const>> parent)
{
    Entity entity{m_registry.create(), this};
    entity.addComponent<IdComponent>(uuid);
    entity.addComponent<TagComponent>(generateUniqueName(name));
    entity.addComponent<TransformComponent>();
    entity.addComponent<DirtyComponent>();

    if (!parent.has_value())
    {
        entity.addComponent<RelationshipComponent>();
    }
    else
    {
        entity.addComponent<RelationshipComponent>(parent.value().get(), entity, entity);
    }

    m_entityNameMap[entity.getComponent<TagComponent>().tag] = static_cast<uint32_t>(entity);
    return entity;
}

// TODO: NEED TO UPDATE THE LOGIC OF GETTING NAMES. DO NOT LIKE THE CURRENT IMPLEMENTATION
/**
 * Cons:
 * 1. Ability to rename has been lost. Need to update this again.
 * 2. Can attach the editorial names to the SceneNode. Makes it heavy and may cause issue when
 * converting SceneGraph to SceneTree.
 */
std::string Scene::generateUniqueName(std::string const& baseName)
{
    auto& count = m_entityNameMap[baseName];
    if (count++ > 0)
    {
        return std::format("{}{}", baseName, count);
    }
    return baseName;
}

void Scene::onTransformUpdated(entt::registry& registry, entt::entity entity)
{
    LY_CORE_ASSERT(registry.any_of<DirtyComponent>(entity), "Transform does not have a dirty component");

    auto& dirty = registry.get<DirtyComponent>(entity);
    dirty.transform = true;
}

void Scene::onCameraUpdated(entt::registry& registry, entt::entity entity)
{
    LY_CORE_ASSERT(registry.any_of<DirtyComponent>(entity), "Camera does not have a dirty component");

    auto& dirty = registry.get<DirtyComponent>(entity);
    dirty.cameraView = true;
    dirty.cameraProjection = true;
}

} // namespace ly::scene
