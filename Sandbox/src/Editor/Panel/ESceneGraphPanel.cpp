#include <Sandbox/Editor/Panel/ESceneGraphPanel.h>

void EESceneGraphPanel::OnImGuiRender() {
    ImGui::Begin("Scene Graph");
    AddEntityButton();
    DrawSceneTree();
    ImGui::End();
}

void EESceneGraphPanel::AddEntityButton() const {
    ImGui::BeginChild("Add Entity", ImVec2(0, 30), false);
    if (ImGui::Button("Add Entity", ImVec2(-FLT_MIN, 0))) {
        m_Scene->CreateEntity(std::to_string(ly::UUID().Get()));
    }
    ImGui::EndChild();
}

void EESceneGraphPanel::DrawSceneTree() {
    ImGui::BeginChild("Scene Hierarchy", ImVec2(0, 0), true);
    if (const auto shared = m_SceneTree.lock()) {
        DrawSceneTreeNode(shared);
    }
    ImGui::EndChild();
}

void EESceneGraphPanel::DrawSceneTreeNode(const ly::Ref<SceneTreeNode>& node) {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

    if (node->Children.empty()) {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }

    if (m_SelectedNode == node) {
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