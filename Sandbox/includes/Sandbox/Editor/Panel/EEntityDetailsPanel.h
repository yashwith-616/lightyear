#pragma once

#include "Lightyear.h"
#include "Sandbox/Editor/Panel/IEditorPanel.h"
#include "Sandbox/Editor/Widgets/WidgetDrawer.h"

class EEntityDetailsPanel : public IEditorPanel {
public:
    EEntityDetailsPanel(std::string name)
        : IEditorPanel(std::move(name)), m_widgetDrawer(ly::makeScope<WidgetDrawer>()) {}

    void onImGuiRender() override;

    void setSelectedEntity(ly::ref<ly::scene::Entity> selectedEntity) { m_selectedEntity = selectedEntity; }

protected:
    void addComponent() const;
    void drawComponentPanel();

private:
    ly::ref<ly::scene::Entity> m_selectedEntity;
    ly::scope<WidgetDrawer> m_widgetDrawer;

    template <typename T>
    void drawComponent() {
        T& component = m_selectedEntity->getComponent<T>();
        ImGui::SeparatorText(typeid(T).name());
        if (m_widgetDrawer->draw(component)) {
            m_selectedEntity->sendUpdateSignal<T>();
        }
    }

    template <typename Tuple, typename Entity, std::size_t... Is>
    void drawAllComponentsImpl(Entity& entity, std::index_sequence<Is...>) {
        ((entity.template hasComponent<std::tuple_element_t<Is, Tuple>>()
              ? drawComponent<std::tuple_element_t<Is, Tuple>>()
              : (void)0),
         ...);
    }

    template <typename Group, typename Entity>
    void drawAllComponents(Entity& entity) {
        using tuple = typename Group::types;
        drawAllComponentsImpl<tuple>(entity, std::make_index_sequence<std::tuple_size_v<tuple>>{});
    }
};