#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Serialization/Text/TextSerialization.h"

namespace ly::scene {

/// \brief Attaches a UUID to the component. All entity present in scene will have this component.
struct LIGHTYEAR_API IDComponent : SerializableContract {
    static constexpr Version version{ 1 };

    UUID ID;

    IDComponent() : ID(UUID()) {}
    explicit IDComponent(const UUID& id) : ID(id) {}

    static void Serialize(TextSerializer& serializer, const IDComponent& component) {
        serializer.Write("version", version);
        serializer.Write("id", component.ID);
    }

    static void Deserialize(TextDeserializer& deserializer, IDComponent& component) {
        Version currVersion{ 0 };
        deserializer.Read("version", currVersion);
        if (version != currVersion) {
            LY_CORE_ASSERT(false, "Version {} and Curr Version has a mismatch! Cannot read file!");
        }
        uint64_t id;
        deserializer.Read("id", id);
        component.ID = UUID(id);
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