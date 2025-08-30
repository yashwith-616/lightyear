#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Serialization/ISerializable.h"

namespace ly::scene {

/**
 * @brief Attaches a UUID to the component. All entity present in scene will have this component.
 */
struct LIGHTYEAR_API IDComponent : ISerializable<IDComponent> {
    static constexpr int Version = 1;

    UUID ID;

    IDComponent() : ID(UUID()) {}
    IDComponent(const UUID& id) : ID(id) {}

    static void Serialize(StreamWriter* serializer, const IDComponent& component) {
        serializer->WriteVersion(Version);
        serializer->WriteRaw(component.ID.Get());
    }

    static void Deserialize(StreamReader* deserializer, IDComponent& component) {
        const int currVersion = deserializer->ReadVersion();
        if (Version != currVersion) {
            LY_CORE_ASSERT(false, "Version {} and Curr Version has a mismatch! Cannot read file!");
        }

        component.ID = UUID(deserializer->ReadRaw<uint64_t>());
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