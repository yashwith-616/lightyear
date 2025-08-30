#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Serialization/ISerializable.h"

namespace ly::scene {

struct LIGHTYEAR_API TagComponent : ISerializable<TagComponent> {
    static constexpr int Version = 1;

    std::string Tag{ kNOTSET };

    TagComponent() = default;
    TagComponent(std::string tag) : Tag(std::move(tag)) {}

    static void Serialize(StreamWriter* serializer, const TagComponent& component) {
        serializer->WriteVersion(Version);
        serializer->WriteString(component.Tag);
    }

    static void Deserialize(StreamReader* deserializer, TagComponent& component) {
        const int kSerializedVersion = deserializer->ReadVersion();
        if (kSerializedVersion != Version) {
            LY_CORE_ASSERT(false, "Serialization Error");
        }
        component.Tag = deserializer->ReadString();
    }
};

}  // namespace ly::scene

LY_DISABLE_WARNINGS_PUSH
#include <refl.hpp>
LY_DISABLE_WARNINGS_POP

REFL_TYPE(ly::scene::TagComponent)
REFL_FIELD(Tag)
REFL_END
