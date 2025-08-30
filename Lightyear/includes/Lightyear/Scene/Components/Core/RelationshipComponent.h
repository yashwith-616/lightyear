#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Core/Scene.h"
#include "Lightyear/Serialization/ISerializable.h"

LY_DISABLE_WARNINGS_PUSH
#include <entt/entt.hpp>
LY_DISABLE_WARNINGS_POP

namespace ly::scene {

struct LIGHTYEAR_API RelationshipComponent : ISerializable<RelationshipComponent> {
    static constexpr int Version = 1;

    std::size_t ChildrenCount{};
    entt::entity Parent{ entt::null };
    entt::entity FirstChild{ entt::null };
    entt::entity NextSibling{ entt::null };
    entt::entity PrevSibling{ entt::null };

    RelationshipComponent() = default;
    RelationshipComponent(entt::entity parent) : Parent(parent) {}
    RelationshipComponent(entt::entity parent, entt::entity nextSibling, entt::entity prevSibling)
        : Parent(parent), NextSibling(nextSibling), PrevSibling(prevSibling) {}

    void SetChild(entt::entity child) { FirstChild = child; }

    static void Serialize(StreamWriter* serializer, const RelationshipComponent& component) {
        serializer->WriteVersion(Version);
        serializer->WriteVersion(component.ChildrenCount);
    }

    static void Deserialize(StreamReader* deserializer, RelationshipComponent& component) {
        const int kVersion = deserializer->ReadVersion();
        if (Version != kVersion) {
            LY_CORE_ASSERT(false, "Version not supported");
        }
        component.ChildrenCount = deserializer->ReadRaw<size_t>();
    }
};

}  // namespace ly::scene