#pragma once

#include <Sandbox/Editor/Comman/SceneTreeNode.h>
#include <lypch.h>
#include "IEditorPanel.h"
#include "Lightyear.h"
#include "Sandbox/Editor/Widgets/WidgetDrawer.h"

class ESceneGraphPanelExp : public IEditorPanel {
public:
    ESceneGraphPanelExp(std::string_view name) : IEditorPanel(name) {}
    virtual ~ESceneGraphPanelExp() = default;

    virtual void OnImGuiRender() override;

    inline ly::WeakRef<SceneTreeNode> GetSelectedNode() { return m_SelectedNode; }
    inline void SetSceneTree(ly::Ref<SceneTreeNode> sceneTree) { m_SceneTree = sceneTree; }

protected:
    void DrawSceneTree();
    void DrawSceneTreeNode(ly::Ref<SceneTreeNode> node);

private:
    ly::WeakRef<SceneTreeNode> m_SceneTree{};
    ly::Ref<ly::scene::Entity> m_SelectedEntity{};
    ly::WeakRef<SceneTreeNode> m_SelectedNode{};
};