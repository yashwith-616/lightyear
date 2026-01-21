#include "Lightyear/Scene/Systems/Camera/CameraSystem.h"
#include "Lightyear/Scene/Components/Camera/CameraComponent.h"
#include "Lightyear/Scene/Components/Camera/CameraTag.h"
#include "Lightyear/Scene/Components/Core/DirtyComponent.h"
#include "Lightyear/Scene/Components/Transform/TransformComponent.h"

LY_DISABLE_WARNINGS_PUSH
#include <entt/entt.hpp>
LY_DISABLE_WARNINGS_POP

namespace ly::scene
{

CameraSystem::CameraSystem() : ISystem("Singleton Camera", SystemLayer::Gameplay) {}

void CameraSystem::init(entt::registry& registry) {}

void CameraSystem::execute(entt::registry& registry)
{
    auto view = registry.view<CameraComponent, TransformComponent, DirtyComponent>();

    for (auto&& [entity, camera, transform, dirty] : view.each())
    {
        if (!registry.any_of<MainCameraTag, EditorCameraTag>(entity))
        {
            continue;
        }

        if (dirty.cameraProjection)
        {
            recalculateProjectionMatrix(camera);
            dirty.cameraProjection = false;
        }

        if (dirty.cameraView || dirty.transform)
        {
            recalculateViewMatrix(camera, transform);
            dirty.cameraView = false;
            dirty.transform = false;
        }
    }
}

void CameraSystem::shutdown(entt::registry& registry) {}

/**
 * Recalculate ViewMatrix and cached matrix
 *
 * @param camera the camera
 * @param transform the transform
 */
void CameraSystem::recalculateViewMatrix(CameraComponent& camera, TransformComponent const& transform)
{
    glm::mat4 const rotationMat = glm::toMat4(glm::quat(transform.rotation));
    glm::mat4 const newTransform = glm::translate(glm::mat4(1.0f), transform.translation) * rotationMat;

    camera.viewMatrix = glm::inverse(newTransform);
    camera.cacheViewProjectionMatrix = camera.projectionMatrix * camera.viewMatrix;
}

/**
 * Recalculate the projection and cached matrix depending on the camera projection type
 *
 * @param camera the camera component
 */
void CameraSystem::recalculateProjectionMatrix(CameraComponent& camera)
{
    float const kOrthoHalfHeight = camera.orthographicSize * 0.5f;
    float const kOrthoHalfWidth = kOrthoHalfHeight * camera.aspectRatio;

    switch (camera.projectionType)
    {
    case CameraProjectionType::Orthographic:
        {
            camera.projectionMatrix = glm::ortho(
                -kOrthoHalfWidth,
                kOrthoHalfWidth,
                -kOrthoHalfHeight,
                kOrthoHalfHeight,
                camera.nearClip,
                camera.farClip);
            break;
        }
    case CameraProjectionType::Perspective:
        {
            camera.projectionMatrix =
                glm::perspective(camera.fovRadians, camera.aspectRatio, camera.nearClip, camera.farClip);

            /*
              TODO: Depth buffer precision[Need checking]: camera.ProjectionMatrix[1][1] *= -1;
                   Needed for GL coordinate system (optional)
            */
            break;
        }
    default:
        LY_CORE_ASSERT(false, "Unsupported projection type!");
    }

    camera.cacheViewProjectionMatrix = camera.projectionMatrix * camera.viewMatrix;
}

} // namespace ly::scene
