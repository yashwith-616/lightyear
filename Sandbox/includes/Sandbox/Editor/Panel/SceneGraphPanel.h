#pragma once

#include <lypch.h>
#include "IEditorPanel.h"
#include "Lightyear.h"
#include "Sandbox/Editor/Widgets/EWidgetDrawer.h"

class SceneGraphPanel : public IEditorPanel {
public:
    virtual void OnImGuiRender() override;

    void SetScene(ly::Ref<ly::scene::Scene> scene) { m_Scene = scene; }

private:
    ly::Ref<ly::scene::Scene> m_Scene{};
    ly::Ref<ly::scene::Entity> m_SelectedEntity{};

private:
    template <typename T>
    void drawComponent(const char* componentName) {
        T& component = m_SelectedEntity->GetComponent<T>();
        if (ImGui::TreeNodeEx(componentName, ImGuiTreeNodeFlags_Framed)) {
            EWidgetDrawer drawWidget;
            drawWidget(component);
            ImGui::TreePop();
        }
    }
};