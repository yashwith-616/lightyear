#include "Lightyear/Scene/SceneGraph.h"

namespace ly::scene {

SceneGraph::SceneGraph() {
    m_DirtyFlag.Resize(1);
    m_DirtyFlag.Set(0);
}

uint32_t SceneGraph::AddNode(const entt::entity entity, const int parentIndex) {
    SceneNode node = SceneNode(entity, parentIndex, {});
    return AddNode(std::move(node));
}

void SceneGraph::MoveNode(const uint32_t nodeIndex, const int parentNodeIndex) {
    LY_CORE_ASSERT(IsValidNode(nodeIndex), "Invalid node index!");
    LY_CORE_ASSERT(IsValidParent(parentNodeIndex), "Invalid parent index!");

    DetachChildNode(nodeIndex);
    m_Nodes[parentNodeIndex].children.push_back(nodeIndex);
    m_Nodes[nodeIndex].parent = parentNodeIndex;

    m_DirtyFlag.Set(nodeIndex);
    m_DirtyFlag.Set(parentNodeIndex);
}

void SceneGraph::RemoveNode(const uint32_t nodeIndex) {
    LY_CORE_ASSERT(IsValidNode(nodeIndex), "Invalid node index!");
    DetachChildNode(nodeIndex);
    ClearNode(nodeIndex);
}

/**
 * @brief Add a SceneNode to the graph. If any FreeSlot is present in the DAG use them, else extend
 * the vector to support the new SceneNode
 *
 * @param sceneNode the new node to be added
 * @return The new index where the node was added to
 */
uint32_t SceneGraph::AddNode(SceneNode&& sceneNode) {
    LY_CORE_ASSERT(IsValidParent(sceneNode.parent), "Invalid parent index!");

    uint32_t index;
    int parentIndex = sceneNode.parent;

    if (!m_FreeSlots.empty()) {
        index = m_FreeSlots.back();
        m_FreeSlots.pop_back();
        m_Nodes[index] = std::move(sceneNode);
    } else {
        index = m_Nodes.size();
        m_Nodes.emplace_back(std::move(sceneNode));
    }

    m_Nodes[parentIndex].children.push_back(index);

    m_DirtyFlag.Resize(std::max(static_cast<size_t>(index + 1), m_DirtyFlag.size()));
    m_DirtyFlag.Set(index);

    return index;
}

/**
 * @brief Remove parent to child relationship.
 * @param sceneNode the child node
 */
void SceneGraph::DetachChildNode(const uint32_t nodeIndex) {
    SceneNode& sceneNode = m_Nodes[nodeIndex];

    LY_CORE_ASSERT(IsValidParent(sceneNode.parent) && sceneNode.parent != nodeIndex,
                   "Invalid Parent Node!");

    // Swap and remove
    auto& children = m_Nodes[sceneNode.parent].children;

    auto it = std::find(children.begin(), children.end(), nodeIndex);
    if (it != children.end()) {
        std::iter_swap(it, children.end() - 1);
        children.pop_back();
    }

    sceneNode.parent = -1;
}

/**
 * @brief Clear the memory for node and it's children recursively. Add the cleared indexes to freed
 * slot.
 * @param sceneNode the scene Node which needs to be cleared
 */
void SceneGraph::ClearNode(const uint32_t nodeIndex) {
    LY_CORE_ASSERT(IsValidNode(nodeIndex), "Invalid node!");

    SceneNode& sceneNode = m_Nodes[nodeIndex];

    for (uint32_t childNodeIndex : sceneNode.children) {
        ClearNode(childNodeIndex);
    }

    sceneNode.children.clear();
    sceneNode.entity = entt::null;
    sceneNode.parent = -1;
    m_FreeSlots.emplace_back(nodeIndex);

    m_DirtyFlag.Reset(nodeIndex);
}

void SceneGraph::MarkDirty(uint32_t nodeIndex) {
    LY_CORE_ASSERT(nodeIndex < m_DirtyFlag.size(),
                   "ScenGraph::MarkDirty - Index out of bounds error");
    m_DirtyFlag.Set(nodeIndex);
}

void SceneGraph::ClearDirty(uint32_t nodeIndex) {
    LY_CORE_ASSERT(nodeIndex < m_DirtyFlag.size(),
                   "ScenGraph::ClearDirty - Index out of bounds error");
    m_DirtyFlag.Reset(nodeIndex);
}

bool SceneGraph::IsDirty(uint32_t nodeIndex) const {
    LY_CORE_ASSERT(nodeIndex < m_DirtyFlag.size(),
                   "ScenGraph::IsDirty - Index out of bounds error");

    return m_DirtyFlag.Test(nodeIndex);
}

bool SceneGraph::IsValidNode(const uint32_t nodeIndex) const {
    return m_Nodes.size() > nodeIndex && m_Nodes[nodeIndex].entity != entt::null;
}

bool SceneGraph::IsValidParent(const int parentIndex) const {
    return m_Nodes.size() > parentIndex && parentIndex >= 0 &&
           m_Nodes[parentIndex].entity != entt::null;
}

}  // namespace ly::scene