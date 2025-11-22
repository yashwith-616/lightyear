#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Components/ComponentRegistry.h"
#include "Lightyear/Scene/Core/Scene.h"

LY_DISABLE_WARNINGS_PUSH
#include <entt/entt.hpp>
LY_DISABLE_WARNINGS_POP

namespace ly::scene {

struct LIGHTYEAR_API RelationshipComponent {
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

    template <class Archive>
    void save(Archive& archive) const {
        archive(cereal::make_nvp("ChildrenCount", ChildrenCount)
                // Save entities as raw IDs (uint32_t or uint64_t), not entt::entity directly
                /*cereal::make_nvp("Parent", static_cast<uint32_t>(Parent)),
                cereal::make_nvp("FirstChild", static_cast<uint32_t>(FirstChild)),
                cereal::make_nvp("NextSibling", static_cast<uint32_t>(NextSibling)),
                cereal::make_nvp("PrevSibling", static_cast<uint32_t>(PrevSibling))*/
        );
    }

    template <class Archive>
    void load(Archive& archive) {
        // uint32_t parent{}, firstChild{}, nextSibling{}, prevSibling{};

        archive(cereal::make_nvp("ChildrenCount", ChildrenCount)
                /*cereal::make_nvp("Parent", parent),
                cereal::make_nvp("FirstChild", firstChild),
                cereal::make_nvp("NextSibling", nextSibling),
                cereal::make_nvp("PrevSibling", prevSibling)*/
        );

        /*Parent     = static_cast<entt::entity>(parent);
        FirstChild = static_cast<entt::entity>(firstChild);
        NextSibling = static_cast<entt::entity>(nextSibling);
        PrevSibling = static_cast<entt::entity>(prevSibling);*/
    }

    SERIALIZE_BODY(RelationshipComponent);
};

}  // namespace ly::scene

REGISTER_COMPONENT(ly::scene::RelationshipComponent, "RelationshipComponent")
