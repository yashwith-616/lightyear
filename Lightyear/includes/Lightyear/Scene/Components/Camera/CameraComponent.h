#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Serialization/Text/TextSerialization.h"

namespace ly::scene {

enum class CameraProjectionType : uint8_t { PERSPECTIVE = BIT(0), ORTHOGRAPHIC = BIT(1) };

struct LIGHTYEAR_API CameraComponent : SerializableContract {
    static constexpr Version version{ 1 };

    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;
    glm::mat4 Cache_ViewProjectionMatrix;

    float OrthographicSize{ kOrthographicCameraSize };  // Only Used in orthographic camera
    float AspectRatio{ kDefaultAspectRatio };
    float FOVRadians{ kDefaultFOVRadians };
    float NearClip{ kDefaultNearClip };
    float FarClip{ kDefaultFarClip };
    CameraProjectionType ProjectionType{ CameraProjectionType::PERSPECTIVE };

    static void Serialize(TextSerializer& serializer, const CameraComponent& camera) {
        serializer.Write("version", version.get());
        serializer.Write("orthographic_size", camera.OrthographicSize);
        serializer.Write("aspect_ratio", camera.AspectRatio);
        serializer.Write("fov_radians", camera.FOVRadians);
        serializer.Write("near_clip", camera.NearClip);
        serializer.Write("far_clip", camera.FarClip);
        serializer.Write("projection_type", static_cast<uint8_t>(camera.ProjectionType));
    }

    static void Deserialize(TextDeserializer& deserializer, CameraComponent& camera) {
        uint64_t currVersion{ 0 };
        deserializer.Read("version", currVersion);
        if (version.get() != currVersion) {
            LY_CORE_ASSERT(
                false, "Version {} and CurrVersion {} has a mismatch! Cannot read file!", version.get(), currVersion);
        }
        deserializer.Read("orthographic_size", camera.OrthographicSize);
        deserializer.Read("aspect_ratio", camera.AspectRatio);
        deserializer.Read("fov_radians", camera.FOVRadians);
        deserializer.Read("near_clip", camera.NearClip);
        deserializer.Read("far_clip", camera.FarClip);

        uint8_t projectionType{ 0 };
        deserializer.Read("projection_type", projectionType);
        camera.ProjectionType = static_cast<CameraProjectionType>(projectionType);
    }
};

}  // namespace ly::scene

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