#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Components/ComponentRegistry.h"

LY_DISABLE_WARNINGS_PUSH
#include "entt/entt.hpp"
LY_DISABLE_WARNINGS_POP

namespace ly::scene {

enum class CameraProjectionType : uint8_t { PERSPECTIVE = 0, ORTHOGRAPHIC = 1 };

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

    template <class Archive>
    void serialize(Archive& archive) {
        uint8_t kProjectionType = static_cast<uint8_t>(ProjectionType);

        archive(cereal::make_nvp("OrthographicSize", OrthographicSize),
                cereal::make_nvp("AspectRatio", AspectRatio),
                cereal::make_nvp("FOVRadians", FOVRadians),
                cereal::make_nvp("NearClip", NearClip),
                cereal::make_nvp("FarClip", FarClip),
                cereal::make_nvp("ProjectionType", kProjectionType));

        if constexpr (Archive::is_loading::value) {
            ProjectionType = static_cast<CameraProjectionType>(kProjectionType);
        }
    }

    SERIALIZE_BODY(CameraComponent)
};

}  // namespace ly::scene

REGISTER_COMPONENT(ly::scene::CameraComponent, "CameraComponent");

LY_DISABLE_WARNINGS_PUSH
#include <refl.hpp>
LY_DISABLE_WARNINGS_POP

REFL_TYPE(ly::scene::CameraComponent)
REFL_FIELD(AspectRatio)
REFL_FIELD(FOVRadians)
REFL_FIELD(NearClip)
REFL_FIELD(FarClip)
REFL_FIELD(ProjectionType)
REFL_END