#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Components/ComponentRegistry.h"

namespace ly::scene {

struct LIGHTYEAR_API TransformComponent {
    glm::vec3 Translation{ glm::vec3(0.0f) };
    glm::vec3 Rotation{ glm::vec3(0.0f) };
    glm::vec3 Scale{ glm::vec3(1.0f) };

    TransformComponent() = default;
    explicit TransformComponent(const glm::vec3& translation) : Translation(translation) {}

    [[nodiscard]] glm::mat4 GetTransform() const {
        glm::mat4 const rotation = glm::toMat4(glm::quat(Rotation));
        return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
    }

    template <class Archive>
    void serialize(Archive& archive) {
        archive(cereal::make_nvp("Translation", Translation),
                cereal::make_nvp("Rotation", Rotation),
                cereal::make_nvp("Scale", Scale));
    }

    SERIALIZE_BODY(TransformComponent)
};

}  // namespace ly::scene

REGISTER_COMPONENT(ly::scene::TransformComponent, "TransformComponent");

LY_DISABLE_WARNINGS_PUSH
#include <refl.hpp>
LY_DISABLE_WARNINGS_POP

REFL_TYPE(ly::scene::TransformComponent)
REFL_FIELD(Translation)
REFL_FIELD(Rotation)
REFL_FIELD(Scale)
REFL_END