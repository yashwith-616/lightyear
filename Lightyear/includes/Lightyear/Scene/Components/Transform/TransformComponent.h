#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Components/ComponentRegistry.h"

namespace ly::scene
{

struct LIGHTYEAR_API TransformComponent
{
    glm::vec3 translation{glm::vec3(0.0f)};
    glm::vec3 rotation{glm::vec3(0.0f)};
    glm::vec3 scale{glm::vec3(1.0f)};

    TransformComponent() = default;
    explicit TransformComponent(glm::vec3 const& translation) : translation(translation) {}

    [[nodiscard]] glm::mat4 getTransform() const
    {
        glm::mat4 const rot = glm::toMat4(glm::quat(rotation));
        return glm::translate(glm::mat4(1.0f), translation) * rot * glm::scale(glm::mat4(1.0f), scale);
    }

    template <class Archive>
    void save(Archive& archive) const
    {
        archive(
            cereal::make_nvp("Translation", translation),
            cereal::make_nvp("Rotation", rotation),
            cereal::make_nvp("Scale", scale));
    }

    template <class Archive>
    void load(Archive& archive)
    {
        archive(
            cereal::make_nvp("Translation", translation),
            cereal::make_nvp("Rotation", rotation),
            cereal::make_nvp("Scale", scale));
    }

    SERIALIZE_BODY(TransformComponent)
};

} // namespace ly::scene

REGISTER_COMPONENT(ly::scene::TransformComponent, "TransformComponent");

LY_DISABLE_WARNINGS_PUSH
#include <refl.hpp>
LY_DISABLE_WARNINGS_POP

REFL_TYPE(ly::scene::TransformComponent)
REFL_FIELD(translation)
REFL_FIELD(rotation)
REFL_FIELD(scale)
REFL_END
