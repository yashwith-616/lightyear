#include "Sandbox/Editor/Workspace/ESceneWorkspace.h"
#include <entt/entt.hpp>

using sceneComponent =
    ly::scene::ComponentGroup<ly::scene::TagComponent, ly::scene::IdComponent, ly::scene::RelationshipComponent>;

void ESceneWorkspace::onAttach(ly::ref<GlobalEditorContext> globalContext) {
    m_globalContext      = globalContext;
    m_sceneGraphPanel    = ly::makeScope<EeSceneGraphPanel>(getPanelTitle(EEditorPanel::SceneGraph));
    m_viewportPanel      = ly::makeScope<EViewportPanel>(getPanelTitle(EEditorPanel::Viewport));
    m_entityDetailsPanel = ly::makeScope<EEntityDetailsPanel>(getPanelTitle(EEditorPanel::Inspector));
}

void ESceneWorkspace::onEvent(ly::Event& event) {}

void ESceneWorkspace::onUpdate(float deltaTime) {}

void ESceneWorkspace::onEditorUpdate() {
    m_sceneTree.reset();
    buildSceneTree();

    m_sceneGraphPanel->setSceneTree(m_sceneTree);
    m_sceneGraphPanel->setScene(m_globalContext->activeScene);
    m_viewportPanel->setFramebuffer(m_globalContext->sceneFramebuffer);
    m_viewportPanel->setSceneRuntime(m_globalContext->sceneRuntime);

    m_selectedNode = m_sceneGraphPanel->getSelectedNode();
    if (auto selected = m_selectedNode.lock()) {
        m_entityDetailsPanel->setSelectedEntity(
            ly::makeRef<ly::scene::Entity>(selected->entity, m_globalContext->activeScene.get()));
    }
}

void ESceneWorkspace::onImGuiRender() {
    drawDockspace();

    if (!isDockspaceInitialized()) {
        setupLayout();
        m_bIsInitialized = true;
    }

    m_sceneGraphPanel->onImGuiRender();
    m_viewportPanel->onImGuiRender();
    m_entityDetailsPanel->onImGuiRender();
}

void ESceneWorkspace::drawDockspace() {
    ImGuiViewport const* viewport = ImGui::GetMainViewport();
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

    m_dockspaceId = ImGui::GetID("World Workspace");
    ImGui::DockSpace(m_dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
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
void ESceneWorkspace::setupLayout() const {
    if (ImGui::DockBuilderGetNode(m_dockspaceId) != nullptr) {
        return;
    }

    ImGui::DockBuilderAddNode(m_dockspaceId, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(m_dockspaceId, ImGui::GetMainViewport()->Size);

    ImGuiID left{};
    ImGuiID right{};
    ImGuiID topRight{};
    ImGuiID bottomRight{};

    constexpr float verticalSplitRatio   = 0.2f;
    constexpr float horizontalSplitRatio = 0.5f;

    ImGui::DockBuilderSplitNode(m_dockspaceId, ImGuiDir_Right, verticalSplitRatio, &right, &left);
    ImGui::DockBuilderSplitNode(left, ImGuiDir_Down, horizontalSplitRatio, &bottomRight, &topRight);

    ImGui::DockBuilderDockWindow(getPanelTitle(EEditorPanel::Viewport).data(), left);
    ImGui::DockBuilderDockWindow(getPanelTitle(EEditorPanel::SceneGraph).data(), topRight);
    ImGui::DockBuilderDockWindow(getPanelTitle(EEditorPanel::Inspector).data(), bottomRight);

    ImGui::DockBuilderFinish(m_dockspaceId);
}

void ESceneWorkspace::buildSceneTree() {
    auto const& registry = getScene().getRegistry();

    m_sceneTree = ly::makeRef<SceneTreeNode>("root", ly::Uuid(0), entt::null);

    for (auto&& [entity, tag, id, relation] : ly::scene::ComponentGroupView<sceneComponent>::view(registry).each()) {
        if (relation.parent != entt::null) {
            continue;
        }
        m_sceneTree->addChild(buildSceneTreeRecursive(entity));
    }
}

ly::ref<SceneTreeNode> ESceneWorkspace::buildSceneTreeRecursive(entt::entity entity) {
    auto const& registry      = getScene().getRegistry();
    auto [tag, uid, relation] = ly::scene::ComponentGroupGet<sceneComponent>::get(registry, entity);

    ly::ref<SceneTreeNode> head = ly::makeRef<SceneTreeNode>(tag.tag, uid.id, entity);

    entt::entity curr = relation.firstChild;
    for (uint32_t i = 0; i < relation.childrenCount; ++i) {
        head->addChild(buildSceneTreeRecursive(curr));

        auto const& childRelation = registry.get<ly::scene::RelationshipComponent>(curr);
        curr                      = childRelation.nextSibling;
    }

    return head;
}

std::string ESceneWorkspace::getPanelTitle(EEditorPanel editorPanel) {
    switch (editorPanel) {
        case EEditorPanel::Viewport:
            return "Viewport";
        case EEditorPanel::SceneGraph:
            return "SceneGraph";
        case EEditorPanel::Inspector:
            return "Inspector";
        case EEditorPanel::PlaceActor:
            return "PlaceActor";
        default:
            return "None";
    };
}