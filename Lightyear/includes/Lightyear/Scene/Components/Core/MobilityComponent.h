#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Serialization/ISerializable.h"

namespace ly::scene {

enum class EMobilityType { STATIC, STATIONARY, MOVABLE };

/**
 * @brief Used for optimization. Describes if an entity in the scene is static or not
 */
struct LIGHTYEAR_API MobilityComponent : ISerializable<MobilityComponent> {
    static constexpr int Version = 1;

    EMobilityType MobilityType{ EMobilityType::STATIC };

    MobilityComponent() = default;
    MobilityComponent(EMobilityType mobilityType) : MobilityType(mobilityType) {}

    MobilityComponent(const MobilityComponent&)            = default;
    MobilityComponent& operator=(const MobilityComponent&) = default;

    static void Serialize(StreamWriter* serializer, MobilityComponent& component) {
        serializer->WriteVersion(Version);
        serializer->WriteRaw(static_cast<uint8_t>(component.MobilityType));
    }

    static void Deserialize(StreamReader* deserializer, MobilityComponent& component) {
        const int kVersion = deserializer->ReadVersion();
        if (kVersion != Version) {
            LY_CORE_ASSERT(false, "Invalid chunk version");
        }

        component.MobilityType = static_cast<EMobilityType>(deserializer->ReadRaw<uint8_t>());
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
