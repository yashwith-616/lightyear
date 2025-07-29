#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly::scene {

enum class CameraProjectionType : uint8_t { PERSPECTIVE = BIT(0), ORTHOGRAPHIC = BIT(1) };

struct LIGHTYEAR_API CameraComponent {
    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;
    glm::mat4 Cache_ViewProjectionMatrix;

    float OrthographicSize{ kOrthographicCameraSize };  // Only Used in orthographic camera
    float AspectRatio{ kDefaultAspectRatio };
    float FOVRadians{ kDefaultFOVRadians };
    float NearClip{ kDefaultNearClip };
    float FarClip{ kDefaultFarClip };
    CameraProjectionType ProjectionType{ CameraProjectionType::PERSPECTIVE };
};

}  // namespace ly::scene

LY_DISABLE_WARNINGS_PUSH
#include <refl.hpp>
LY_DISABLE_WARNINGS_POP

REFL_TYPE(ly::scene::CameraComponent)
REFL_FIELD(ProjectionMatrix)
REFL_FIELD(ViewMatrix)
REFL_FIELD(AspectRatio)
REFL_FIELD(FOVRadians)
REFL_FIELD(NearClip)
REFL_FIELD(FarClip)
REFL_FIELD(ProjectionType)
REFL_END