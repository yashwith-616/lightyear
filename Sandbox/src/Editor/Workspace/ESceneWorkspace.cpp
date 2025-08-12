#include "Sandbox/Editor/Workspace/ESceneWorkspace.h"
#include <entt/entt.hpp>

using SceneComponent =
    ly::scene::ComponentGroup<ly::scene::TagComponent, ly::scene::IDComponent, ly::scene::RelationshipComponent>;

void ESceneWorkspace::OnAttach(ly::Ref<GlobalEditorContext> globalContext) {
    m_GlobalContext      = globalContext;
    m_SceneGraphPanel    = ly::MakeScope<EESceneGraphPanel>(GetPanelTitle(EEditorPanel::SCENE_GRAPH));
    m_ViewportPanel      = ly::MakeScope<EViewportPanel>(GetPanelTitle(EEditorPanel::VIEWPORT));
    m_EntityDetailsPanel = ly::MakeScope<EEntityDetailsPanel>(GetPanelTitle(EEditorPanel::INSPECTOR));
}

void ESceneWorkspace::OnEvent(ly::Event& event) {}

void ESceneWorkspace::OnUpdate(float deltaTime) {}

void ESceneWorkspace::OnEditorUpdate() {
    m_SceneTree.reset();
    BuildSceneTree();

    m_SceneGraphPanel->SetSceneTree(m_SceneTree);
    m_ViewportPanel->SetFramebuffer(m_GlobalContext->SceneFramebufferId);
    m_SelectedNode = m_SceneGraphPanel->GetSelectedNode();
    if (auto selected = m_SelectedNode.lock()) {
        LY_LOG(ly::LogType::Info, "Selected a node in scene graph with name {}", selected->Name);
        m_EntityDetailsPanel->SetSelectedEntity(
            ly::MakeRef<ly::scene::Entity>(selected->Entity, m_GlobalContext->ActiveScene.get()));
    }
}

void ESceneWorkspace::OnImGuiRender() {
    DrawDockspace();

    if (!IsDockspaceInitialized()) {
        SetupDockspace();
        m_bIsInitialized = true;
    }

    m_SceneGraphPanel->OnImGuiRender();
    m_ViewportPanel->OnImGuiRender();
    m_EntityDetailsPanel->OnImGuiRender();
}

void ESceneWorkspace::DrawDockspace() {
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    if (viewport == nullptr) {
        LY_LOG(ly::LogType::Error, "Main Viewport is null!");
        return;
    }

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                                             ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                                             ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
                                             ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    ImGui::Begin("World Workspace", nullptr, windowFlags);
    ImGui::PopStyleVar(2);

    m_DockspaceID = ImGui::GetID("World Workspace");
    ImGui::DockSpace(m_DockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
    ImGui::End();
}

// TODO: Currently setup for a fixed layout
/**
 * 1. Need to use EditorPreset so the layout can be changed as per requirement
 * 2. EditorLayout need to be cached based on requirement
 * 3. All the panels eligible this Workspace needs to be added to menuBar > Windows
 * 4. Layouts for all window will be distributed to all window beforehand.
 * 5. Panel will have property to hide them. Panel need an enum based dirty flag
 */
void ESceneWorkspace::SetupDockspace() const {
    if (ImGui::DockBuilderGetNode(m_DockspaceID) != nullptr) {
        ImGui::DockBuilderRemoveNode(m_DockspaceID);
    }

    ImGui::DockBuilderAddNode(m_DockspaceID, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(m_DockspaceID, ImGui::GetMainViewport()->Size);

    ImGuiID left{};
    ImGuiID right{};
    ImGuiID topRight{};
    ImGuiID bottomRight{};

    constexpr float verticalSplitRatio   = 0.2;
    constexpr float horizontalSplitRatio = 0.5;

    ImGui::DockBuilderSplitNode(m_DockspaceID, ImGuiDir::ImGuiDir_Right, verticalSplitRatio, &right, &left);
    ImGui::DockBuilderSplitNode(left, ImGuiDir::ImGuiDir_Down, horizontalSplitRatio, &bottomRight, &topRight);

    ImGui::DockBuilderDockWindow(GetPanelTitle(EEditorPanel::VIEWPORT).data(), left);
    ImGui::DockBuilderDockWindow(GetPanelTitle(EEditorPanel::SCENE_GRAPH).data(), topRight);
    ImGui::DockBuilderDockWindow(GetPanelTitle(EEditorPanel::INSPECTOR).data(), bottomRight);

    ImGui::DockBuilderFinish(m_DockspaceID);
}

void ESceneWorkspace::BuildSceneTree() {
    const auto& registry = GetScene().GetRegistry();

    m_SceneTree = ly::MakeRef<SceneTreeNode>("root", ly::UUID(0), entt::null);

    for (auto&& [entity, tag, id, relation] : ly::scene::ComponentGroupView<SceneComponent>::view(registry).each()) {
        if (relation.Parent != entt::null) {
            continue;
        }
        m_SceneTree->AddChild(BuildSceneTreeRecursive(entity));
    }
}

ly::Ref<SceneTreeNode> ESceneWorkspace::BuildSceneTreeRecursive(entt::entity entity) {
    const auto& registry      = GetScene().GetRegistry();
    auto [tag, uid, relation] = ly::scene::ComponentGroupGet<SceneComponent>::get(registry, entity);

    ly::Ref<SceneTreeNode> head = ly::MakeRef<SceneTreeNode>(tag.Tag, uid.ID, entity);

    entt::entity curr = relation.FirstChild;
    for (uint32_t i = 0; i < relation.ChildrenCount; ++i) {
        head->AddChild(BuildSceneTreeRecursive(curr));

        const auto& childRelation = registry.get<ly::scene::RelationshipComponent>(curr);
        curr                      = childRelation.NextSibling;
    }

    return head;
}

std::string ESceneWorkspace::GetPanelTitle(EEditorPanel editorPanel) {
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