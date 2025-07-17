#pragma once

#include <Lightyear.h>

/**
 * @brief ImGUI uses the following struct to render the scene tree.
 *
 * enable_shared_from_this does not attomically increment when sharing
 * the following scene that has already been created.
 *
 * TODO: Require a dirty flag in Entity Registry.
 * Allows to dirty an entity and reuse the remaining tree as it is.
 * By default dirty should be set to true
 */
class SceneTreeNode : public std::enable_shared_from_this<SceneTreeNode> {
public:
    std::string Name{ "none" };
    ly::UUID Id{};
    entt::entity Entity{};

    ly::WeakRef<SceneTreeNode> Parent{};
    std::vector<ly::Ref<SceneTreeNode>> Children{};

    SceneTreeNode(std::string name, ly::UUID uid, entt::entity entity)
        : Name(std::move(name)), Id(uid), Entity(entity) {}

    void AddChild(ly::Ref<SceneTreeNode> child) {
        child->Parent = shared_from_this();
        Children.push_back(std::move(child));
    }

    void RemoveChild(const ly::UUID& id) {
        const auto it =
            std::ranges::remove_if(Children, [&](const ly::Ref<SceneTreeNode>& child) {
                return child->Id == id;
            }).begin();
        Children.erase(it, Children.end());
    }
};