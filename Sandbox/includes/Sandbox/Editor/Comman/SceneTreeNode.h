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
class SceneTreeNode : public std::enable_shared_from_this<SceneTreeNode>
{
public:
    std::string name{"none"};
    ly::Uuid id{};
    entt::entity entity{};

    ly::weakRef<SceneTreeNode> parent{};
    std::vector<ly::ref<SceneTreeNode>> children{};

    SceneTreeNode(std::string name, ly::Uuid uid, entt::entity entity) : name(std::move(name)), id(uid), entity(entity)
    {}

    void addChild(ly::ref<SceneTreeNode> child)
    {
        child->parent = shared_from_this();
        children.push_back(std::move(child));
    }

    void removeChild(ly::Uuid const& id)
    {
        auto const it = std::ranges::remove_if(children, [&](ly::ref<SceneTreeNode> const& child) {
                            return child->id == id;
                        }).begin();
        children.erase(it, children.end());
    }
};
