#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Components/ComponentRegistry.h"

namespace ly::scene {

struct LIGHTYEAR_API TagComponent {
    std::string tag{ k_kNotset };

    TagComponent() = default;
    explicit TagComponent(std::string tag) : tag(std::move(tag)) {}

    template <class Archive>
    void save(Archive& archive) const {
        archive(cereal::make_nvp("Tag", tag));
    }

    template <class Archive>
    void load(Archive& archive) {
        archive(cereal::make_nvp("Tag", tag));
    }

    SERIALIZE_BODY(TagComponent)
};

}  // namespace ly::scene

REGISTER_COMPONENT(ly::scene::TagComponent, "TagComponent");

LY_DISABLE_WARNINGS_PUSH
#include <refl.hpp>
LY_DISABLE_WARNINGS_POP

REFL_TYPE(ly::scene::TagComponent)
REFL_FIELD(tag)
REFL_END
