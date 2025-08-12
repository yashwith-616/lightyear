#pragma once

#include "Lightyear.h"
#include "Sandbox/Editor/Panel/IEditorPanel.h"
#include "Sandbox/Editor/Widgets/SimpleWidgetDrawer.h"

class EEntityDetailsPanel : public IEditorPanel {
public:
    EEntityDetailsPanel(std::string name) : IEditorPanel(std::move(name)), m_WidgetDrawer(ly::MakeScope<SimpleWidgetDrawer>()) {}

    void OnImGuiRender() override;

    void SetSelectedEntity(ly::Ref<ly::scene::Entity> selectedEntity) {
        m_SelectedEntity = selectedEntity;
    }

private:
    ly::Ref<ly::scene::Entity> m_SelectedEntity;
    ly::Scope<SimpleWidgetDrawer> m_WidgetDrawer;

    template<typename T>
    void DrawComponent() {
        T& component = m_SelectedEntity->GetComponent<T>();
        m_WidgetDrawer->Draw(component);
    }
};