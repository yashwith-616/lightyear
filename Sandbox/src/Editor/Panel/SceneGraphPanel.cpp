#include "Sandbox/Editor/Panel/SceneGraphPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

/**
 * @brief Needs fix - Not well optimized
 */
void SceneGraphPanel::OnImGuiRender() {
    ImGui::Begin("ECS Editor");

    auto windowSize = ImGui::GetContentRegionAvail();

    ImGui::BeginChild("Entities", { windowSize.x * 0.3f, windowSize.y });
    auto view = m_Scene->GetAllEntitiesWith<ly::scene::TagComponent>();

    for (auto entity : view) {
        const auto& tag = view.get<ly::scene::TagComponent>(entity);
        bool isSelected = m_SelectedEntity && *m_SelectedEntity == entity;
        if (ImGui::Selectable(tag.Tag.c_str(), isSelected)) {
            m_SelectedEntity = ly::MakeRef<ly::scene::Entity>(entity, m_Scene.get());
        }
    }

    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Entity", { windowSize.x * 0.7f, windowSize.y });
    if (m_SelectedEntity) {
        drawComponent<ly::scene::TagComponent>("Tag");
        drawComponent<ly::scene::IDComponent>("Id");
        drawComponent<ly::scene::TransformComponent>("Transform");
    }
    ImGui::EndChild();

    ImGui::End();
}
