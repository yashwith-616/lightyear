#pragma once

#include <Lightyear.h>
#include <lypch.h>

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
    ly::uuid Id{};
    entt::entity Entity{};

    ly::WeakRef<SceneTreeNode> Parent{};
    std::vector<ly::Ref<SceneTreeNode>> Children{};

    SceneTreeNode(const std::string& name, ly::uuid uid, entt::entity entity)
        : Name(name), Id(uid), Entity(entity) {}

    void AddChild(ly::Ref<SceneTreeNode> child) {
        child->Parent = shared_from_this();
        Children.push_back(std::move(child));
    }

    void RemoveChild(const ly::uuid& id) {
        auto it = std::remove_if(
            Children.begin(), Children.end(), [&](const ly::Ref<SceneTreeNode>& child) {
                return child->Id == id;
            });
        Children.erase(it, Children.end());
    }
};