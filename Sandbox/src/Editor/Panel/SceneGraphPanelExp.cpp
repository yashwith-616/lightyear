#include <Sandbox/Editor/Panel/SceneGraphPanelExp.h>

void ESceneGraphPanelExp::OnImGuiRender() {
    DrawSceneTree();
}

void ESceneGraphPanelExp::DrawSceneTree() {
    ImGui::Begin("Scene Hierarchy");
    if (auto shared = m_SceneTree.lock()) {
        DrawSceneTreeNode(shared);
    }
    ImGui::End();
}

void ESceneGraphPanelExp::DrawSceneTreeNode(ly::Ref<SceneTreeNode> node) {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

    if (node->Children.empty()) flags |= ImGuiTreeNodeFlags_Leaf;

    if (m_SelectedNode.lock() == node) flags |= ImGuiTreeNodeFlags_Selected;

    bool opened = ImGui::TreeNodeEx(
        reinterpret_cast<void*>(static_cast<uintptr_t>(node->Id)), flags, "%s", node->Name.c_str());

    // Handle selection
    if (ImGui::IsItemClicked()) {
        m_SelectedNode = node;
    }

    // Draw children recursively
    if (opened) {
        for (ly::Ref<SceneTreeNode> child : node->Children) {
            DrawSceneTreeNode(child);
        }
        ImGui::TreePop();
    }
}