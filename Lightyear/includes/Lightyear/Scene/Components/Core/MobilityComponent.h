#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly::scene {

enum class EMobilityType { STATIC, STATIONARY, MOVABLE };

/**
 * @brief Used for optimization. Describes if an entity in the scene is static or not
 */
struct LIGHTYEAR_API MobilityComponent {
    EMobilityType MobilityType{ EMobilityType::STATIC };

    MobilityComponent() = default;
    MobilityComponent(EMobilityType mobilityType) : MobilityType(mobilityType) {}

    MobilityComponent(const MobilityComponent&)            = default;
    MobilityComponent& operator=(const MobilityComponent&) = default;
};

}  // namespace ly::scene

LY_DISABLE_WARNINGS_PUSH
#include <refl.hpp>
LY_DISABLE_WARNINGS_POP

// Will be removed
REFL_TYPE(ly::scene::MobilityComponent)
REFL_FIELD(MobilityType)
REFL_END
