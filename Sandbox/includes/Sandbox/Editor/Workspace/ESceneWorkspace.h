#pragma once

#include <imgui.h>
#include "Sandbox/Editor/Workspace/IEditorWorkspace.h"

/**
 * @brief ImGUI uses the following struct to render the scene.
 *
 * TODO: Require a dirty flag in Entity Registry.
 * Allows to dirty an entity and reuse the remaining tree as it is.
 * By default dirty should be set to true
 */
struct SceneTreeNode {
    std::string Name{ "none" };
    ly::uuid UUID{};
    entt::entity Entity{};
    SceneTreeNode* Parent{};
    std::vector<SceneTreeNode*> Children{};

    bool HasChildren{ false };

    SceneTreeNode(std::string name,
                  ly::uuid uid,
                  entt::entity entity,
                  SceneTreeNode* parent,
                  bool hasChildren)
        : Name(name), UUID(uid), Entity(entity), Parent(parent), HasChildren(hasChildren) {}
};

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
    enum class EEditorPanel : uint8_t { VIEWPORT = 0, SCENE_GRAPH, INSPECTOR, PLACE_ACTOR, MAX };

protected:
    ImGuiID m_DockspaceID{};

    void SetupDockspace();
    void DrawSceneTree();
    void DrawSceneTreeNode(SceneTreeNode* node);

    void BuildSceneTree();
    SceneTreeNode* BuildSceneTreeRecursive(entt::entity entity, SceneTreeNode* parent);

    inline bool IsDockspaceInitialized() { return m_bIsMinimized; }
    inline std::string_view GetPanelTitle(EEditorPanel editorPanel);
    inline const ly::scene::Scene& GetScene() {
        LY_CORE_ASSERT(m_GlobalContext && m_GlobalContext->m_ActiveScene, "Active Scene is null!");
        return *(m_GlobalContext->m_ActiveScene);
    }

private:
    bool m_bIsInitiatlized{ false };
    ly::Ref<GlobalEditorContext> m_GlobalContext{};
    std::vector<SceneTreeNode*> m_SceneTree{};
    SceneTreeNode* m_SelectedNode{};
};