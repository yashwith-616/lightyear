#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly::scene {

struct LIGHTYEAR_API TagComponent {
    std::string Tag{ kNOTSET };

    TagComponent() = default;
    TagComponent(std::string tag) : Tag(std::move(tag)) {}
};

}  // namespace ly::scene

LY_DISABLE_WARNINGS_PUSH
#include <refl.hpp>
LY_DISABLE_WARNINGS_POP

REFL_TYPE(ly::scene::TagComponent)
REFL_FIELD(Tag)
REFL_END
