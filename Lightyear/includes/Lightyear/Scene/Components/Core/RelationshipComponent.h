#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Core/Scene.h"
#include "Lightyear/Serialization/Text/TextSerialization.h"

LY_DISABLE_WARNINGS_PUSH
#include <entt/entt.hpp>
LY_DISABLE_WARNINGS_POP

namespace ly::scene {

struct LIGHTYEAR_API RelationshipComponent : SerializableContract {
    static constexpr Version version{ 1 };

    std::size_t ChildrenCount{};
    entt::entity Parent{ entt::null };
    entt::entity FirstChild{ entt::null };
    entt::entity NextSibling{ entt::null };
    entt::entity PrevSibling{ entt::null };

    RelationshipComponent() = default;
    explicit RelationshipComponent(entt::entity parent) : Parent(parent) {}
    RelationshipComponent(entt::entity parent, entt::entity nextSibling, entt::entity prevSibling)
        : Parent(parent), NextSibling(nextSibling), PrevSibling(prevSibling) {}

    void SetChild(entt::entity child) { FirstChild = child; }

    static void Serialize(TextSerializer& serializer, const RelationshipComponent& component) {
        serializer.Write("version", version);
        serializer.Write("children_count", component.ChildrenCount);
    }

    static void Deserialize(TextDeserializer& deserializer, RelationshipComponent& component) {
        Version currVersion{ 0 };
        deserializer.Read("version", currVersion);
        if (version != currVersion) {
            LY_CORE_ASSERT(false, "Version not supported");
        }
        deserializer.Read("children_count", component.ChildrenCount);
    }
};

}  // namespace ly::scene