#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Serialization/ISerializable.h"

namespace ly::scene {

struct LIGHTYEAR_API TransformComponent : ISerializable<TransformComponent> {
    static constexpr int Version = 1;

    glm::vec3 Translation{ glm::vec3(0.0f) };
    glm::vec3 Rotation{ glm::vec3(0.0f) };
    glm::vec3 Scale{ glm::vec3(1.0f) };

    TransformComponent() = default;
    TransformComponent(const glm::vec3& translation) : Translation(translation) {}

    [[nodiscard]] glm::mat4 GetTransform() const {
        glm::mat4 const rotation = glm::toMat4(glm::quat(Rotation));
        return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
    }

    static void Serialize(StreamWriter* serializer, const TransformComponent& component) {
        serializer->WriteVersion(Version);
        serializer->WriteRaw(component.Translation);
        serializer->WriteRaw(component.Rotation);
        serializer->WriteRaw(component.Scale);
    }

    static void Deserialize(StreamReader* deserialize, TransformComponent& component) {
        const int kSerializedVersion = deserialize->ReadVersion();
        if (Version == kSerializedVersion) {
            LY_CORE_ASSERT(false, "Serialization error");
        }
        component.Translation = deserialize->ReadRaw<glm::vec3>();
        component.Rotation    = deserialize->ReadRaw<glm::vec3>();
        component.Scale       = deserialize->ReadRaw<glm::vec3>();
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