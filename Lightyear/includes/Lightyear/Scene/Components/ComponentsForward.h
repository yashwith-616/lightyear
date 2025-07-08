#pragma once

#include "Lightyear/Renderer/Camera/SceneCamera.h"
#include "Lightyear/Renderer/Primitives/Shader.h"
#include "Lightyear/Renderer/Primitives/Texture.h"
#include "Lightyear/Renderer/Primitives/VertexArray.h"

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

/**
 * @brief Need a bitset component that indicates all the component that can exists
 */
namespace ly::scene {

enum class EMobilityType { STATIC, STATIONARY, MOVABLE };

/**
 * @brief Attaches an UUID to the component. All entity present in scene will have this component.
 */
struct IDComponent {
    UUID ID;

    IDComponent() : ID(UUID()) {}
    IDComponent(const UUID& id) : ID(id) {}
};

/**
 * @brief Used for naming a component. All entity present in the scene will have this component
 */
struct TagComponent {
    CName Tag{ "Default" };

    TagComponent() = default;
    TagComponent(const CName& tag) : Tag(tag) {}
};

/**
 * @brief Used for optimization. Describes if an entity in the scene is static or not
 */
struct MobilityComponent {
    EMobilityType MobilityType{ EMobilityType::STATIC };

    MobilityComponent() = default;
    MobilityComponent(EMobilityType mobilityType) : MobilityType(mobilityType) {}
    MobilityComponent(const MobilityComponent&) = default;
};

struct TransformComponent {
    glm::vec3 Translation{ glm::vec3(0.0f) };
    glm::vec3 Rotation{ glm::vec3(0.0f) };
    glm::vec3 Scale{ glm::vec3(1.0f) };

    TransformComponent() = default;
    TransformComponent(const glm::vec3& translation) : Translation(translation) {}

    glm::mat4 GetTransform() const {
        glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
        return glm::translate(glm::mat4(1.0f), Translation) * rotation *
               glm::scale(glm::mat4(1.0f), Scale);
    }
};

// TODO: Needs rework on Camera component as a whole
struct CameraComponent {
    Ref<renderer::SceneCamera> Camera;
    bool bIsPrimary{ true };

    CameraComponent() : Camera(MakeRef<renderer::SceneCamera>(DEFAULT_ASPECT_RATIO)) {}
    CameraComponent(Ref<renderer::SceneCamera> camera) : Camera(camera) {}
    CameraComponent(const CameraComponent&)            = default;
    CameraComponent& operator=(const CameraComponent&) = default;
};

struct MeshComponent {
    Ref<renderer::VertexArray> MeshAsset;
    Ref<renderer::Shader> ShaderAsset;
    Ref<renderer::Texture> TextureAsset;

    MeshComponent(Ref<renderer::VertexArray> vertexArray,
                  Ref<renderer::Shader> shader,
                  Ref<renderer::Texture> texture)
        : MeshAsset(vertexArray), ShaderAsset(shader), TextureAsset(texture) {}

    MeshComponent(const MeshComponent&) = default;
};

struct RenderComponent {
    bool bIsHidden{ false };
    bool bIsHiddenEditor{ false };

    RenderComponent() = default;
};

struct RelationshipComponent {
    std::size_t ChildrenCount{};
    entt::entity Parent{ entt::null };
    entt::entity FirstChild{ entt::null };
    entt::entity NextSibling{ entt::null };
    entt::entity PrevSibling{ entt::null };

    RelationshipComponent() {}
    RelationshipComponent(entt::entity parent) : Parent(parent) {}
    RelationshipComponent(entt::entity parent, entt::entity nextSibling, entt::entity prevSibling)
        : Parent(parent), NextSibling(nextSibling), PrevSibling(prevSibling) {}

    inline void SetChild(entt::entity child) { FirstChild = child; }
};

}  // namespace ly::scene