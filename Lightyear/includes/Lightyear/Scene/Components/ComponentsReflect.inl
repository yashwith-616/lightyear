#pragma once

#include <refl.hpp>
#include "ComponentsForward.h"

REFL_TYPE(ly::scene::TransformComponent)
REFL_FIELD(Translation)
REFL_FIELD(Rotation)
REFL_FIELD(Scale)
REFL_END

REFL_TYPE(ly::scene::IDComponent)
REFL_FIELD(ID)
REFL_END

REFL_TYPE(ly::scene::TagComponent)
REFL_FIELD(Tag)
REFL_END