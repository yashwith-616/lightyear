#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Serialization/Serialization.h"

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

    static void Serialize(StreamWriter& serializer, const CameraComponent& camera) {
        serializer.WriteVersion(version);
        serializer.WriteRaw(camera.OrthographicSize);
        serializer.WriteRaw(camera.AspectRatio);
        serializer.WriteRaw(camera.FOVRadians);
        serializer.WriteRaw(camera.NearClip);
        serializer.WriteRaw(camera.FarClip);
        serializer.WriteRaw(camera.ProjectionType);
    }

    static void Deserialize(StreamReader& deserializer, CameraComponent& camera) {
        const Version currVersion = deserializer.ReadVersion();
        if (version != currVersion) {
            LY_CORE_ASSERT(false, "Version {} and Curr Version has a mismatch! Cannot read file!");
        }
        camera.OrthographicSize = deserializer.ReadRaw<float>();
        camera.AspectRatio      = deserializer.ReadRaw<float>();
        camera.FOVRadians       = deserializer.ReadRaw<float>();
        camera.NearClip         = deserializer.ReadRaw<float>();
        camera.FarClip          = deserializer.ReadRaw<float>();
        camera.ProjectionType   = static_cast<CameraProjectionType>(deserializer.ReadRaw<uint8_t>());
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