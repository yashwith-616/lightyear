#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Serialization/Text/TextSerialization.h"

namespace ly::scene {

enum class EMobilityType { STATIC, STATIONARY, MOVABLE };

/// \brief Used for optimization. Describes if the entity position can be changes
struct LIGHTYEAR_API MobilityComponent : SerializableContract {
    EMobilityType MobilityType{ EMobilityType::STATIC };

    MobilityComponent() = default;
    explicit MobilityComponent(EMobilityType mobilityType) : MobilityType(mobilityType) {}

    MobilityComponent(const MobilityComponent&)            = default;
    MobilityComponent& operator=(const MobilityComponent&) = default;

    static void Serialize(TextSerializer& serializer, MobilityComponent& component) {
        serializer.Write("mobility_type", static_cast<uint8_t>(component.MobilityType));
    }

    static void Deserialize(TextDeserializer& deserializer, MobilityComponent& component) {
        uint8_t mobilityType{ 0 };
        deserializer.Read("mobility_type", mobilityType);
        component.MobilityType = static_cast<EMobilityType>(mobilityType);
    }
};

}  // namespace ly::scene

LY_DISABLE_WARNINGS_PUSH
#include <refl.hpp>
LY_DISABLE_WARNINGS_POP

// Will be removed
REFL_TYPE(ly::scene::MobilityComponent)
REFL_FIELD(MobilityType)
REFL_END
