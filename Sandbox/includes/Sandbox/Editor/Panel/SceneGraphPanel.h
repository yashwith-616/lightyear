#pragma once

#include "IEditorPanel.h"
#include "Lightyear.h"
#include "Sandbox/Editor/Widgets/WidgetDrawer.h"

class SceneGraphPanel : public IEditorPanel {
public:
    SceneGraphPanel(std::string name) : IEditorPanel(std::move(name)) {}

     ~SceneGraphPanel() override = default;

    void OnImGuiRender() override;

    void SetScene(const ly::Ref<ly::scene::Scene> &scene) { m_Scene = scene; }

private:
    ly::Ref<ly::scene::Scene> m_Scene{};
    ly::Ref<ly::scene::Entity> m_SelectedEntity{};

    template <typename T>
    void DrawComponent(const char* componentName) const {
        T& component = m_SelectedEntity->GetComponent<T>();
        if (ImGui::TreeNodeEx(componentName, ImGuiTreeNodeFlags_Framed)) {
            WidgetDrawer drawWidget;
            drawWidget(component);
            ImGui::TreePop();
        }
    }
};