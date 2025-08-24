#pragma once

#include "IEditorPanel.h"
#include "Lightyear.h"
#include "Sandbox/Editor/Comman/SceneTreeNode.h"

class EESceneGraphPanel : public IEditorPanel {
public:
    explicit EESceneGraphPanel(std::string name) : IEditorPanel(std::move(name)) {}
    ~EESceneGraphPanel() override = default;

    void OnImGuiRender() override;

    ly::WeakRef<SceneTreeNode> GetSelectedNode() { return m_SelectedNode; }
    void SetSceneTree(const ly::Ref<SceneTreeNode>& sceneTree) { m_SceneTree = sceneTree; }

protected:
    void DrawSceneTree();
    void DrawSceneTreeNode(const ly::Ref<SceneTreeNode>& node);

private:
    ly::WeakRef<SceneTreeNode> m_SceneTree;
    ly::Ref<SceneTreeNode> m_SelectedNode;
};