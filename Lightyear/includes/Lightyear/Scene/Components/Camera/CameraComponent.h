#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Components/ComponentRegistry.h"

LY_DISABLE_WARNINGS_PUSH
#include "entt/entt.hpp"
LY_DISABLE_WARNINGS_POP

namespace ly::scene
{

enum class CameraProjectionType : uint8_t
{
    Perspective = 0,
    Orthographic = 1
};

struct LIGHTYEAR_API CameraComponent
{
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 cacheViewProjectionMatrix;

    float orthographicSize{k_kOrthographicCameraSize}; // Only Used in orthographic camera
    float aspectRatio{k_kDefaultAspectRatio};
    float fovRadians{k_kDefaultFovRadians};
    float nearClip{k_kDefaultNearClip};
    float farClip{k_kDefaultFarClip};
    CameraProjectionType projectionType{CameraProjectionType::Perspective};

    glm::mat4 multiply() { return projectionMatrix * viewMatrix * cacheViewProjectionMatrix; }

    template <class Archive>
    void save(Archive& archive) const
    {
        uint8_t kProjectionType = static_cast<uint8_t>(projectionType);

        archive(
            cereal::make_nvp("OrthographicSize", orthographicSize),
            cereal::make_nvp("AspectRatio", aspectRatio),
            cereal::make_nvp("FOVRadians", fovRadians),
            cereal::make_nvp("NearClip", nearClip),
            cereal::make_nvp("FarClip", farClip),
            cereal::make_nvp("ProjectionType", kProjectionType));
    }

    template <class Archive>
    void load(Archive& archive)
    {
        uint8_t kProjectionType{};

        archive(
            cereal::make_nvp("OrthographicSize", orthographicSize),
            cereal::make_nvp("AspectRatio", aspectRatio),
            cereal::make_nvp("FOVRadians", fovRadians),
            cereal::make_nvp("NearClip", nearClip),
            cereal::make_nvp("FarClip", farClip),
            cereal::make_nvp("ProjectionType", kProjectionType));

        projectionType = static_cast<CameraProjectionType>(kProjectionType);
    }

    SERIALIZE_BODY(CameraComponent)
};

} // namespace ly::scene

REGISTER_COMPONENT(ly::scene::CameraComponent, "CameraComponent");

LY_DISABLE_WARNINGS_PUSH
#include <refl.hpp>
LY_DISABLE_WARNINGS_POP

REFL_TYPE(ly::scene::CameraComponent)
REFL_FIELD(aspectRatio)
REFL_FIELD(fovRadians)
REFL_FIELD(nearClip)
REFL_FIELD(farClip)
REFL_FIELD(projectionType)
REFL_END
