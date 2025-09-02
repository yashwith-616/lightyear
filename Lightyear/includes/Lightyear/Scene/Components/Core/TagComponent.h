#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Serialization/Serialization.h"

namespace ly::scene {

struct LIGHTYEAR_API TagComponent : SerializableContract {
    static constexpr Version version{ 1 };

    std::string Tag{ kNOTSET };

    TagComponent() = default;
    explicit TagComponent(std::string tag) : Tag(std::move(tag)) {}

    static void Serialize(StreamWriter& serializer, const TagComponent& component) {
        serializer.WriteVersion(version);
        serializer.WriteString(component.Tag);
    }

    static void Deserialize(StreamReader& deserializer, TagComponent& component) {
        const Version currVersion = deserializer.ReadVersion();
        if (currVersion != version) {
            LY_CORE_ASSERT(false, "Serialization Error");
        }
        component.Tag = deserializer.ReadString();
    }
};

}  // namespace ly::scene

LY_DISABLE_WARNINGS_PUSH
#include <refl.hpp>
LY_DISABLE_WARNINGS_POP

REFL_TYPE(ly::scene::TagComponent)
REFL_FIELD(Tag)
REFL_END
