#pragma once

#include <Sandbox/Editor/Comman/SceneTreeNode.h>
#include <Sandbox/Editor/Panel/SceneGraphPanelExp.h>

#include "Sandbox/Editor/Panel/EViewportPanel.h"
#include "Sandbox/Editor/Workspace/IEditorWorkspace.h"

/**
 * @brief Workspace class responsible for rendering SceneTree.
 */
class ESceneWorkspace : IEditorWorkspace {
public:
    explicit ESceneWorkspace(std::string name) : IEditorWorkspace(std::move(name)) {}

    void OnAttach(ly::Ref<GlobalEditorContext> globalContext) override;
    void OnEvent(ly::Event& event) override;
    void OnUpdate(float deltaTime) override;
    void OnEditorUpdate() override;
    void OnImGuiRender() override;

protected:
    enum class EEditorPanel : uint8_t { VIEWPORT = 1, SCENE_GRAPH, INSPECTOR, PLACE_ACTOR, MAX };

    ImGuiID m_DockspaceID{};

    void DrawDockspace();
    void SetupDockspace() const;
    void BuildSceneTree();
    ly::Ref<SceneTreeNode> BuildSceneTreeRecursive(entt::entity entity);

    [[nodiscard]] bool IsDockspaceInitialized() const { return m_bIsInitialized; }

    [[nodiscard]] const ly::scene::Scene& GetScene() const {
        LY_CORE_ASSERT(m_GlobalContext && m_GlobalContext->ActiveScene, "Active Scene is null!");
        return *(m_GlobalContext->ActiveScene);
    }

    static std::string GetPanelTitle(EEditorPanel editorPanel);

private:
    // Move all panels to a registry, along with what they need.
    ly::Scope<ESceneGraphPanelExp> m_SceneGraphPanel;
    ly::Scope<EViewportPanel> m_ViewportPanel;

    bool m_bIsInitialized{ false };
    ly::Ref<GlobalEditorContext> m_GlobalContext;
    ly::Ref<SceneTreeNode> m_SceneTree;
    ly::WeakRef<SceneTreeNode> m_SelectedNode;
};