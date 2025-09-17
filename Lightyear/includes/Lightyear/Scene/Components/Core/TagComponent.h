#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Components/ComponentRegistry.h"
#include "Lightyear/Serialization/Text/TextSerialization.h"

namespace ly::scene {

struct LIGHTYEAR_API TagComponent : SerializableContract {
    static constexpr Version version{ 1 };

    std::string Tag{ kNOTSET };

    TagComponent() = default;
    explicit TagComponent(std::string tag) : Tag(std::move(tag)) {}

    static void Serialize(TextSerializer& serializer, const TagComponent& component) {
        serializer.BeginObject("TagComponent");
        serializer.Write("tag", component.Tag);
        serializer.EndObject();
    }

    static void Deserialize(TextDeserializer& deserializer, TagComponent& component) {
        deserializer.BeginObject("TagComponent");
        deserializer.Read("tag", component.Tag);
        deserializer.EndObject();
    }
};

}  // namespace ly::scene

REGISTER_COMPONENT(ly::scene::TagComponent, "TagComponent");

LY_DISABLE_WARNINGS_PUSH
#include <refl.hpp>
LY_DISABLE_WARNINGS_POP

REFL_TYPE(ly::scene::TagComponent)
REFL_FIELD(Tag)
REFL_END
