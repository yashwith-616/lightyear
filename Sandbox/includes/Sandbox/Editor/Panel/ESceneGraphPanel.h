#pragma once

#include "IEditorPanel.h"
#include "Lightyear.h"
#include "Sandbox/Editor/Comman/SceneTreeNode.h"

class EESceneGraphPanel : public IEditorPanel
{
public:
    explicit EESceneGraphPanel(std::string name) : IEditorPanel(std::move(name)) {}
    ~EESceneGraphPanel() override = default;

    void OnImGuiRender() override;

    ly::WeakRef<SceneTreeNode> GetSelectedNode() { return m_SelectedNode; }
    void SetSceneTree(const ly::Ref<SceneTreeNode>& sceneTree) { m_SceneTree = sceneTree; }
    void SetScene(const ly::Ref<ly::scene::Scene>& scene) { m_Scene = scene; }

protected:
    void DrawSceneTree();
    void AddEntityButton() const;
    void DrawSceneTreeNode(const ly::Ref<SceneTreeNode>& node);

private:
    ly::WeakRef<SceneTreeNode> m_SceneTree;
    ly::Ref<SceneTreeNode> m_SelectedNode;
    ly::Ref<ly::scene::Scene> m_Scene;
};
