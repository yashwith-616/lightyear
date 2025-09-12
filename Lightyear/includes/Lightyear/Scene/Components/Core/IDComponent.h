#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Serialization/Text/TextSerialization.h"

namespace ly::scene {

/// \brief Attaches a UUID to the component. All entity present in scene will have this component.
struct LIGHTYEAR_API IDComponent : SerializableContract {
    UUID ID;

    IDComponent() : ID(UUID()) {}
    explicit IDComponent(const UUID& id) : ID(id) {}

    static void Serialize(TextSerializer& serializer, const IDComponent& component) {
        serializer.Write("id", component.ID);
    }

    static void Deserialize(TextDeserializer& deserializer, IDComponent& component) {
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