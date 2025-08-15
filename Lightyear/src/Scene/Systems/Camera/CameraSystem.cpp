#include "Lightyear/Scene/Systems/Camera/CameraSystem.h"
#include "Lightyear/Scene/Components/Camera/CameraComponent.h"
#include "Lightyear/Scene/Components/Camera/CameraTag.h"
#include "Lightyear/Scene/Components/Core/DirtyComponent.h"
#include "Lightyear/Scene/Components/Transform/TransformComponent.h"

LY_DISABLE_WARNINGS_PUSH
#include <entt/entt.hpp>
LY_DISABLE_WARNINGS_POP

namespace ly::scene {

CameraSystem::CameraSystem() : ISystem("Singleton Camera", SystemLayer::Gameplay) {}

void CameraSystem::Init(entt::registry& registry) {}

void CameraSystem::Execute(entt::registry& registry) {
    auto view = registry.view<CameraComponent, TransformComponent, DirtyComponent>();

    for (auto&& [entity, camera, transform, dirty] : view.each()) {
        if (!registry.any_of<MainCameraTag, EditorCameraTag>(entity)) {
            continue;
        }

        if (dirty.Camera_Projection) {
            RecalculateProjectionMatrix(camera);
            dirty.Camera_Projection = false;
        }

        if (dirty.Camera_View) {
            RecalculateViewMatrix(camera, transform);
            dirty.Camera_View = false;
        }
    }
}

void CameraSystem::Shutdown(entt::registry& registry) {}

/**
 * Recalculate ViewMatrix and cached matrix
 *
 * @param camera the camera
 * @param transform the transform
 */
void CameraSystem::RecalculateViewMatrix(CameraComponent& camera, const TransformComponent& transform) {
    LY_CORE_LOG(LogType::Warn, "Recalculate View Matrix");

    const glm::mat4 rotationMat  = glm::toMat4(glm::quat(transform.Rotation));
    const glm::mat4 newTransform = glm::translate(glm::mat4(1.0f), transform.Translation) * rotationMat;

    camera.ViewMatrix                 = glm::inverse(newTransform);
    camera.Cache_ViewProjectionMatrix = camera.ProjectionMatrix * camera.ViewMatrix;
}

/**
 * Recalculate the projection and cached matrix depending on the camera projection type
 *
 * @param camera the camera component
 */
void CameraSystem::RecalculateProjectionMatrix(CameraComponent& camera) {
    LY_CORE_LOG(LogType::Warn, "Recalculate Projection Matrix");

    const float kOrthoHalfHeight = camera.OrthographicSize * 0.5f;
    const float kOrthoHalfWidth  = kOrthoHalfHeight * camera.AspectRatio;

    switch (camera.ProjectionType) {
        case CameraProjectionType::ORTHOGRAPHIC: {
            camera.ProjectionMatrix = glm::ortho(-kOrthoHalfWidth,
                                                 kOrthoHalfWidth,
                                                 -kOrthoHalfHeight,
                                                 kOrthoHalfHeight,
                                                 camera.NearClip,
                                                 camera.FarClip);
            break;
        }
        case CameraProjectionType::PERSPECTIVE: {
            camera.ProjectionMatrix =
                glm::perspective(camera.FOVRadians, camera.AspectRatio, camera.NearClip, camera.FarClip);

            /*
              TODO: Depth buffer precision[Need checking]: camera.ProjectionMatrix[1][1] *= -1;
                   Needed for GL coordinate system (optional)
            */
            break;
        }
        default:
            LY_CORE_ASSERT(false, "Unsupported projection type!");
    }

    camera.Cache_ViewProjectionMatrix = camera.ProjectionMatrix * camera.ViewMatrix;
}

}  // namespace ly::scene