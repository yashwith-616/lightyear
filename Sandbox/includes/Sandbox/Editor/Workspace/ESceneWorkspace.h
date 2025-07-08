#pragma once

#include <imgui.h>

#include <Sandbox/Editor/Comman/SceneTreeNode.h>
#include <Sandbox/Editor/Panel/SceneGraphPanelExp.h>
#include "Sandbox/Editor/Workspace/IEditorWorkspace.h"

/**
 * @brief Workspace class responsible for rendering SceneTree.
 */
class ESceneWorkspace : IEditorWorkspace {
public:
    ESceneWorkspace(std::string_view name) : IEditorWorkspace(name) {}
    ~ESceneWorkspace() = default;

    virtual void OnAttach(ly::Ref<GlobalEditorContext> globalContext) override;
    virtual void OnEvent(ly::Event& event) override;
    virtual void OnUpdate(float deltaTime) override;
    virtual void OnEditorUpdate() override;
    virtual void OnImGuiRender() override;

protected:
    enum class EEditorPanel : uint8_t { VIEWPORT = 1, SCENE_GRAPH, INSPECTOR, PLACE_ACTOR, MAX };

protected:
    ImGuiID m_DockspaceID{};

    void DrawDockspace();
    void SetupDockspace();
    void BuildSceneTree();
    ly::Ref<SceneTreeNode> BuildSceneTreeRecursive(entt::entity entity);

    inline bool IsDockspaceInitialized() { return m_bIsInitiatlized; }
    inline const ly::scene::Scene& GetScene() {
        LY_CORE_ASSERT(m_GlobalContext && m_GlobalContext->m_ActiveScene, "Active Scene is null!");
        return *(m_GlobalContext->m_ActiveScene);
    }

    static std::string_view GetPanelTitle(EEditorPanel editorPanel);

private:
    ly::Scope<ESceneGraphPanelExp> m_SceneGraphPanel{};

    bool m_bIsInitiatlized{ false };
    ly::Ref<GlobalEditorContext> m_GlobalContext{};
    ly::Ref<SceneTreeNode> m_SceneTree{};
    ly::WeakRef<SceneTreeNode> m_SelectedNode{};
};