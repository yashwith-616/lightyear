#pragma once

#include "IEditorPanel.h"
#include "Lightyear.h"
#include "Sandbox/Editor/Comman/SceneTreeNode.h"

class EeSceneGraphPanel : public IEditorPanel {
public:
    explicit EeSceneGraphPanel(std::string name) : IEditorPanel(std::move(name)) {}
    ~EeSceneGraphPanel() override = default;

    void onImGuiRender() override;

    ly::weakRef<SceneTreeNode> getSelectedNode() { return m_selectedNode; }
    void setSceneTree(ly::ref<SceneTreeNode> const& sceneTree) { m_sceneTree = sceneTree; }
    void setScene(ly::ref<ly::scene::Scene> const& scene) { m_scene = scene; }

protected:
    void drawSceneTree();
    void addEntityButton() const;
    void drawSceneTreeNode(ly::ref<SceneTreeNode> const& node);

private:
    ly::weakRef<SceneTreeNode> m_sceneTree;
    ly::ref<SceneTreeNode> m_selectedNode;
    ly::ref<ly::scene::Scene> m_scene;
};