#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Components/ComponentRegistry.h"
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
        serializer.BeginObject("MobilityComponent");
        serializer.Write("mobility_type", static_cast<uint8_t>(component.MobilityType));
        serializer.EndObject();
    }

    static void Deserialize(TextDeserializer& deserializer, MobilityComponent& component) {
        deserializer.BeginObject("MobilityComponent");
        uint8_t mobilityType{ 0 };
        deserializer.Read("mobility_type", mobilityType);
        component.MobilityType = static_cast<EMobilityType>(mobilityType);
        deserializer.EndObject();
    }
};

}  // namespace ly::scene

REGISTER_COMPONENT(ly::scene::MobilityComponent, "MobilityComponent");

LY_DISABLE_WARNINGS_PUSH
#include <refl.hpp>
LY_DISABLE_WARNINGS_POP

// Will be removed
REFL_TYPE(ly::scene::MobilityComponent)
REFL_FIELD(MobilityType)
REFL_END
