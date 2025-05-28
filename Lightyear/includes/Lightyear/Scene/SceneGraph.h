#pragma once

#include "Lightyear/LightyearCore.h"
#include "entt/entt.hpp"
#include "lypch.h"

namespace ly::scene {

/**
 * @brief Represent each node in the hierarchial data structure.
 */
struct SceneNode {
    entt::entity entity{ entt::null };
    int parent = -1;
    std::vector<uint32_t> children{};

    SceneNode(entt::entity e, int parentIndex, std::vector<uint32_t>&& childIndices)
        : entity(e), parent(parentIndex), children(std::move(childIndices)) {}
};

/**
 * @brief Scene Graph is a hierarchial data structure that holds all the entities present in the
 * scene and thier hierarchial relationship with each other
 *
 * It is implemented using DAG. Can make this class a separate type.
 *
 * OPTIMIZE:
 *  1. m_FreeSlots can use a ringbuffer
 *  2. Make this class a separate type. Add it into a static library.
 */
class LIGHTYEAR_API SceneGraph {
public:
    SceneGraph();

    /**
     * @brief Adding node defaults to parent or root node i.e, 0
     *
     * @param entity the ecs index
     * @param parentIndex the parent ID Index in DAG
     * @return the index at which the node was added
     */
    uint32_t AddNode(const entt::entity entity, const int parentIndex = 0);

    /**
     * @brief Move node with NodeIndex to new parent
     * @param nodeIndex the node need to be moved
     * @param parentNodeIndex the new parent node index
     */
    void MoveNode(const uint32_t nodeIndex, const int parentNodeIndex);

    /**
     * @brief Remove node from scene graph by index
     * @param nodeIndex The index of the node that needs to be removed
     */
    void RemoveNode(const uint32_t nodeIndex);

    /**
     * @brief Mark node as dirty
     * @param nodeIndex the index of the node
     */
    void MarkDirty(uint32_t nodeIndex);

    /**
     * @brief Clear the dirty flag for the node
     * @param nodeIndex the index of the node
     */
    void ClearDirty(uint32_t nodeIndex);

    /**
     * @brief Check if a node is dirty
     *
     * @param nodeIndex the index of the node
     * @return True, if the node is flagged dirty
     */
    bool IsDirty(uint32_t nodeIndex) const;

private:
    std::vector<SceneNode> m_Nodes = { SceneNode(entt::null, -1, {}) };
    std::vector<uint32_t> m_FreeSlots{};
    dynamic_bitset m_DirtyFlag{};

private:
    uint32_t AddNode(SceneNode&& sceneNode);
    void DetachChildNode(const uint32_t nodeIndex);
    void ClearNode(const uint32_t nodeIndex);

    bool IsValidNode(const uint32_t nodeIndex) const;
    bool IsValidParent(const int paretnIndex) const;
};

}  // namespace ly::scene