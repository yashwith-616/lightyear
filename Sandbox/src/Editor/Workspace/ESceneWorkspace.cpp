#include "Sandbox/Editor/Workspace/ESceneWorkspace.h"
#include <entt/entt.hpp>

using SceneComponent = ly::scene::ComponentGroup<ly::scene::TagComponent,
                                                 ly::scene::IDComponent,
                                                 ly::scene::RelationshipComponent>;

void ESceneWorkspace::OnAttach(ly::Ref<GlobalEditorContext> globalContext) {
    m_GlobalContext = globalContext;
}

void ESceneWorkspace::OnEvent(ly::Event& event) {}

void ESceneWorkspace::OnUpdate(float deltaTime) {}

void ESceneWorkspace::OnEditorUpdate() {}

void ESceneWorkspace::OnImGuiRender() {
    /*if (!IsDockspaceInitialized()) {
        SetupDockspace();
    }*/

    m_SceneTree.clear();
    BuildSceneTree();

    //ImGui::DockSpace(m_DockspaceID, ImVec2(0.f, 0.f), ImGuiDockNodeFlags_None);
    DrawSceneTree();
}

/**
 * TODO: Currently setup for a fixed layout
 *
 * 1. Need to use EditorPreset so the layout can be changed as per requirement
 * 2. EditorLayout need to be cached based on requirement
 * 3. All the panels eligible this Workspace needs to be added to menuBar > windows
 * 4. Layouts for all window will be distributed to all window before hand.
 * 5. Panel will have property to hide them. Panel need an enum based dirty flag
 */
void ESceneWorkspace::SetupDockspace() {
    ImGui::DockBuilderRemoveNode(m_DockspaceID);
    ImGui::DockBuilderAddNode(m_DockspaceID, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(m_DockspaceID, ImGui::GetMainViewport()->Size);

    ImGuiID left, right, topRight, bottomRight;
    ImGui::DockBuilderSplitNode(m_DockspaceID, ImGuiDir::ImGuiDir_Right, 0.2, &right, &left);
    ImGui::DockBuilderSplitNode(left, ImGuiDir::ImGuiDir_Down, 0.5, &bottomRight, &topRight);

    ImGui::DockBuilderDockWindow(GetPanelTitle(EEditorPanel::VIEWPORT).data(), left);
    ImGui::DockBuilderDockWindow(GetPanelTitle(EEditorPanel::SCENE_GRAPH).data(), topRight);
    ImGui::DockBuilderDockWindow(GetPanelTitle(EEditorPanel::INSPECTOR).data(), bottomRight);

    ImGui::DockBuilderFinish(m_DockspaceID);

    m_bIsInitiatlized = true;
}

void ESceneWorkspace::BuildSceneTree() {
    auto& registry = GetScene().GetRegistry();

    for (auto [entity, tag, id, relation] :
         ly::scene::ComponentGroupView<SceneComponent>::view(registry).each()) {
        if (relation.Parent != entt::null) {
            continue;
        }

        // Start DFS from this root
        SceneTreeNode* root = BuildSceneTreeRecursive(entity, nullptr);
        m_SceneTree.push_back(root);
    }
}

SceneTreeNode* ESceneWorkspace::BuildSceneTreeRecursive(entt::entity entity,
                                                        SceneTreeNode* parent) {
    auto& registry           = GetScene().GetRegistry();
    auto [tag, id, relation] = ly::scene::ComponentGroupGet<SceneComponent>::get(registry, entity);

    SceneTreeNode* node =
        new SceneTreeNode(tag.Tag, id.ID, entity, parent, relation.ChildrenCount > 0);

    entt::entity curr = relation.FirstChild;
    for (int i = 0; i < relation.ChildrenCount; ++i) {
        SceneTreeNode* childNode = BuildSceneTreeRecursive(curr, node);
        node->Children.push_back(childNode);

        auto& childRelation = registry.get<ly::scene::RelationshipComponent>(curr);
        curr                = childRelation.NextSibling;
    }

    return node;
}

void ESceneWorkspace::DrawSceneTree() {
    ImGui::Begin("Scene Hierarchy");

    for (SceneTreeNode* root : m_SceneTree) {
        DrawSceneTreeNode(root);
    }

    ImGui::End();
}

void ESceneWorkspace::DrawSceneTreeNode(SceneTreeNode* node) {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

    if (node->Children.empty()) flags |= ImGuiTreeNodeFlags_Leaf;

    if (m_SelectedNode == node) flags |= ImGuiTreeNodeFlags_Selected;

    bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<uintptr_t>(node->UUID)),
                                    flags,
                                    "%s",
                                    node->Name.c_str());

    // Handle selection
    if (ImGui::IsItemClicked()) {
        m_SelectedNode = node;
    }

    // Draw children recursively
    if (opened) {
        for (SceneTreeNode* child : node->Children) {
            DrawSceneTreeNode(child);
        }
        ImGui::TreePop();
    }
}

std::string_view ESceneWorkspace::GetPanelTitle(EEditorPanel editorPanel) {
    switch (editorPanel) {
        case EEditorPanel::VIEWPORT:
            return "Viewport";
        case EEditorPanel::SCENE_GRAPH:
            return "SceneGraph";
        case EEditorPanel::INSPECTOR:
            return "Inspector";
        case EEditorPanel::PLACE_ACTOR:
            return "PlaceActor";
        default:
            return "None";
    };
}