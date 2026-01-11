#pragma once

#include "Lightyear.h"
#include "Sandbox/Editor/Panel/IEditorPanel.h"
#include "Sandbox/Editor/Widgets/WidgetDrawer.h"

class EEntityDetailsPanel : public IEditorPanel
{
public:
    explicit EEntityDetailsPanel(std::string name) : IEditorPanel(std::move(name)), m_WidgetDrawer(ly::MakeScope<WidgetDrawer>())
    {}

    void OnImGuiRender() override;

    void SetSelectedEntity(ly::Ref<ly::scene::Entity> const& selectedEntity) { m_SelectedEntity = selectedEntity; }

protected:
    void AddComponent() const;
    void DrawComponentPanel();

private:
    ly::Ref<ly::scene::Entity> m_SelectedEntity;
    ly::Scope<WidgetDrawer> m_WidgetDrawer;

    template <typename T>
    void DrawComponent()
    {
        T& component = m_SelectedEntity->GetComponent<T>();
        ImGui::SeparatorText(typeid(T).name());
        if (m_WidgetDrawer->Draw(component))
            m_SelectedEntity->SendUpdateSignal<T>();
    }

    template <typename Tuple, typename Entity, std::size_t... Is>
    void DrawAllComponentsImpl(Entity& entity, std::index_sequence<Is...>)
    {
        ((entity.template HasComponent<std::tuple_element_t<Is, Tuple>>()
              ? DrawComponent<std::tuple_element_t<Is, Tuple>>()
              : (void) 0),
         ...);
    }

    template <typename Group, typename Entity>
    void DrawAllComponents(Entity& entity)
    {
        using Tuple = typename Group::Types;
        DrawAllComponentsImpl<Tuple>(entity, std::make_index_sequence<std::tuple_size_v<Tuple>>{});
    }
};
