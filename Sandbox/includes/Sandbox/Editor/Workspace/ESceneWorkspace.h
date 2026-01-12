#pragma once

#include <Sandbox/Editor/Comman/SceneTreeNode.h>
#include <Sandbox/Editor/Panel/ESceneGraphPanel.h>

#include "Sandbox/Editor/Panel/EEntityDetailsPanel.h"
#include "Sandbox/Editor/Panel/EViewportPanel.h"
#include "Sandbox/Editor/Workspace/IEditorWorkspace.h"

/**
 * @brief Workspace class responsible for rendering SceneTree.
 */
class ESceneWorkspace : IEditorWorkspace {
public:
    explicit ESceneWorkspace(std::string name) : IEditorWorkspace(std::move(name)) {}

    void onAttach(ly::ref<GlobalEditorContext> globalContext) override;
    void onEvent(ly::Event& event) override;
    void onUpdate(float deltaTime) override;
    void onEditorUpdate() override;
    void onImGuiRender() override;

protected:
    enum class EEditorPanel : uint8_t { Viewport = 1, SceneGraph, Inspector, PlaceActor, Max };

    ImGuiID m_dockspaceId{};

    void drawDockspace();
    void setupLayout() const;
    void buildSceneTree();
    ly::ref<SceneTreeNode> buildSceneTreeRecursive(entt::entity entity);

    [[nodiscard]] bool isDockspaceInitialized() const { return m_bIsInitialized; }

    [[nodiscard]] ly::scene::Scene const& getScene() const {
        LY_CORE_ASSERT(m_globalContext && m_globalContext->activeScene, "Active Scene is null!");
        return *(m_globalContext->activeScene);
    }

    static std::string getPanelTitle(EEditorPanel editorPanel);

private:
    // Move all panels to a registry, along with what they need.
    ly::scope<EeSceneGraphPanel> m_sceneGraphPanel;
    ly::scope<EViewportPanel> m_viewportPanel;
    ly::scope<EEntityDetailsPanel> m_entityDetailsPanel;

    bool m_bIsInitialized{ false };
    ly::ref<GlobalEditorContext> m_globalContext;
    ly::ref<SceneTreeNode> m_sceneTree;
    ly::weakRef<SceneTreeNode> m_selectedNode;
};