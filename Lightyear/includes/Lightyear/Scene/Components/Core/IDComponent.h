#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Serialization/Serialization.h"

namespace ly::scene {

/// \brief Attaches a UUID to the component. All entity present in scene will have this component.
struct LIGHTYEAR_API IDComponent : SerializableContract {
    static constexpr Version version{1};

    UUID ID;

    IDComponent() : ID(UUID()) {}
    explicit IDComponent(const UUID& id) : ID(id) {}

    static void Serialize(StreamWriter& serializer, const IDComponent& component) {
        serializer.WriteVersion(version);
        serializer.WriteRaw(component.ID.Get());
    }

    static void Deserialize(StreamReader& deserializer, IDComponent& component) {
        const Version currVersion = deserializer.ReadVersion();
        if (version != currVersion) {
            LY_CORE_ASSERT(false, "Version {} and Curr Version has a mismatch! Cannot read file!");
        }

        component.ID = UUID(deserializer.ReadRaw<uint64_t>());
    }
};

}  // namespace ly::scene

LY_DISABLE_WARNINGS_PUSH
#include <refl.hpp>
LY_DISABLE_WARNINGS_POP

// Will be removed
REFL_TYPE(ly::scene::IDComponent)
REFL_FIELD(ID)
REFL_END