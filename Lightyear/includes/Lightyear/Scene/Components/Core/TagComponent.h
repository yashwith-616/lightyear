#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Components/ComponentRegistry.h"

namespace ly::scene
{

struct LIGHTYEAR_API TagComponent
{
    std::string Tag{kNOTSET};

    TagComponent() = default;
    explicit TagComponent(std::string tag) : Tag(std::move(tag)) {}

    template <class Archive>
    void save(Archive& archive) const
    {
        archive(cereal::make_nvp("Tag", Tag));
    }

    template <class Archive>
    void load(Archive& archive)
    {
        archive(cereal::make_nvp("Tag", Tag));
    }

    SERIALIZE_BODY(TagComponent)
};

} // namespace ly::scene

REGISTER_COMPONENT(ly::scene::TagComponent, "TagComponent");

LY_DISABLE_WARNINGS_PUSH
#include <refl.hpp>
LY_DISABLE_WARNINGS_POP

REFL_TYPE(ly::scene::TagComponent)
REFL_FIELD(Tag)
REFL_END
