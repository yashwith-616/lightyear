#include <Sandbox/Editor/Panel/ESceneGraphPanel.h>

void EeSceneGraphPanel::onImGuiRender()
{
    ImGui::Begin("Scene Graph");
    addEntityButton();
    drawSceneTree();
    ImGui::End();
}

void EeSceneGraphPanel::drawSceneTree()
{
    ImGui::BeginChild("Scene Hierarchy", ImVec2(0, 0), true);
    if (auto const shared = m_sceneTree.lock())
    {
        drawSceneTreeNode(shared);
    }
    ImGui::EndChild();
}

void EeSceneGraphPanel::addEntityButton() const
{
    ImGui::BeginChild("Add Entity", ImVec2(0, 30), false);
    if (ImGui::Button("Add Entity", ImVec2(-FLT_MIN, 0)))
    {
        m_scene->createEntity(std::to_string(ly::Uuid().get()));
    }
    ImGui::EndChild();
}

void EeSceneGraphPanel::drawSceneTreeNode(ly::ref<SceneTreeNode> const& node)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

    if (node->children.empty())
    {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }

    if (m_selectedNode == node)
    {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    // NOLINTNEXTLINE
    const bool hasOpened = ImGui::TreeNodeEx(reinterpret_cast<void*>(node->id.get()), flags, "%s", node->name.c_str());

    // Handle selection
    if (ImGui::IsItemClicked())
    {
        m_selectedNode = node;
    }

    // Draw children recursively
    if (hasOpened)
    {
        for (ly::ref<SceneTreeNode> const& child : node->children)
        {
            drawSceneTreeNode(child);
        }
        ImGui::TreePop();
    }
}
