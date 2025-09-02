#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Serialization/Serialization.h"

namespace ly::scene {

enum class EMobilityType { STATIC, STATIONARY, MOVABLE };

/// \brief Used for optimization. Describes if the entity position can be changes
struct LIGHTYEAR_API MobilityComponent : SerializableContract {
    static constexpr Version version{1};

    EMobilityType MobilityType{ EMobilityType::STATIC };

    MobilityComponent() = default;
    explicit MobilityComponent(EMobilityType mobilityType) : MobilityType(mobilityType) {}

    MobilityComponent(const MobilityComponent&)            = default;
    MobilityComponent& operator=(const MobilityComponent&) = default;

    static void Serialize(StreamWriter& serializer, MobilityComponent& component) {
        serializer.WriteVersion(version);
        serializer.WriteRaw(static_cast<uint8_t>(component.MobilityType));
    }

    static void Deserialize(StreamReader& deserializer, MobilityComponent& component) {
        const Version currVersion = deserializer.ReadVersion();
        if (version != currVersion) {
            LY_CORE_ASSERT(false, "Invalid chunk version");
        }

        component.MobilityType = static_cast<EMobilityType>(deserializer.ReadRaw<uint8_t>());
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
