#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Serialization/Text/TextSerialization.h"

namespace ly::scene {

struct LIGHTYEAR_API TransformComponent : SerializableContract {
    static constexpr Version version{ 1 };

    glm::vec3 Translation{ glm::vec3(0.0f) };
    glm::vec3 Rotation{ glm::vec3(0.0f) };
    glm::vec3 Scale{ glm::vec3(1.0f) };

    TransformComponent() = default;
    explicit TransformComponent(const glm::vec3& translation) : Translation(translation) {}

    [[nodiscard]] glm::mat4 GetTransform() const {
        glm::mat4 const rotation = glm::toMat4(glm::quat(Rotation));
        return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
    }

    static void Serialize(TextSerializer& serializer, const TransformComponent& component) {
        serializer.Write("version", version);
        serializer.Write("transform", component.Translation);
        serializer.Write("rotation", component.Rotation);
        serializer.Write("scale", component.Scale);
    }

    static void Deserialize(TextDeserializer& deserializer, TransformComponent& component) {
        Version currVersion{ 0 };
        deserializer.Read("version", currVersion);
        if (version == currVersion) {
            LY_CORE_ASSERT(false, "Serialization error");
        }
        deserializer.Read("transform", component.Translation);
        deserializer.Read("rotation", component.Rotation);
        deserializer.Read("scale", component.Scale);
    }
};

}  // namespace ly::scene

LY_DISABLE_WARNINGS_PUSH
#include <refl.hpp>
LY_DISABLE_WARNINGS_POP

REFL_TYPE(ly::scene::TransformComponent)
REFL_FIELD(Translation)
REFL_FIELD(Rotation)
REFL_FIELD(Scale)
REFL_END