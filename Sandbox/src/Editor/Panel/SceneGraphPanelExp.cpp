#include <Sandbox/Editor/Panel/SceneGraphPanelExp.h>

void ESceneGraphPanelExp::OnImGuiRender() {
    DrawSceneTree();
}

void ESceneGraphPanelExp::DrawSceneTree() {
    ImGui::Begin("Scene Hierarchy");
    if (const auto shared = m_SceneTree.lock()) {
        DrawSceneTreeNode(shared);
    }
    ImGui::End();
}

void ESceneGraphPanelExp::DrawSceneTreeNode(const ly::Ref<SceneTreeNode>& node) {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

    if (node->Children.empty()) {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }

    if (m_SelectedNode.lock() == node) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    // NOLINTNEXTLINE
    const bool hasOpened = ImGui::TreeNodeEx(reinterpret_cast<void*>(node->Id.Get()), flags, "%s", node->Name.c_str());

    // Handle selection
    if (ImGui::IsItemClicked()) {
        m_SelectedNode = node;
    }

    // Draw children recursively
    if (hasOpened) {
        for (const ly::Ref<SceneTreeNode>& child : node->Children) {
            DrawSceneTreeNode(child);
        }
        ImGui::TreePop();
    }
}