#pragma once

#include "Sandbox/Editor/Comman/SceneTreeNode.h"
#include "IEditorPanel.h"
#include "Lightyear.h"
#include "Sandbox/Editor/Widgets/WidgetDrawer.h"

class ESceneGraphPanelExp : public IEditorPanel {
public:
    explicit ESceneGraphPanelExp(std::string name) : IEditorPanel(std::move(name)) {}
    ~ESceneGraphPanelExp() override = default;

    void OnImGuiRender() override;

    ly::WeakRef<SceneTreeNode> GetSelectedNode() { return m_SelectedNode; }
    void SetSceneTree(const ly::Ref<SceneTreeNode> &sceneTree) { m_SceneTree = sceneTree; }

protected:
    void DrawSceneTree();
    void DrawSceneTreeNode(const ly::Ref<SceneTreeNode>& node);

private:
    ly::WeakRef<SceneTreeNode> m_SceneTree;
    ly::Ref<ly::scene::Entity> m_SelectedEntity;
    ly::WeakRef<SceneTreeNode> m_SelectedNode;
};