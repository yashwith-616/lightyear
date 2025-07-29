#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly::scene {

/**
 * @brief Attaches a UUID to the component. All entity present in scene will have this component.
 */
struct LIGHTYEAR_API IDComponent {
    UUID ID;

    IDComponent() : ID(UUID()) {}
    IDComponent(const UUID& id) : ID(id) {}
};

}  // namespace ly::scene

LY_DISABLE_WARNINGS_PUSH
#include <refl.hpp>
LY_DISABLE_WARNINGS_POP

// Will be removed
REFL_TYPE(ly::scene::IDComponent)
REFL_FIELD(ID)
REFL_END