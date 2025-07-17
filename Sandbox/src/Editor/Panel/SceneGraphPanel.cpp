#include "Sandbox/Editor/Panel/SceneGraphPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

/**
 * @brief Needs fix - Not well optimized
 */
void SceneGraphPanel::OnImGuiRender() {
    ImGui::Begin("ECS Editor");

    auto windowSize = ImGui::GetContentRegionAvail();

    constexpr float entitiesVerticalSplitRatio = 0.3f;
    ImGui::BeginChild("Entities", { windowSize.x * entitiesVerticalSplitRatio, windowSize.y });
    auto view = m_Scene->GetAllEntitiesWith<ly::scene::TagComponent>();

    for (auto entity : view) {
        const auto& tag       = view.get<ly::scene::TagComponent>(entity);
        const bool isSelected = m_SelectedEntity && *m_SelectedEntity == entity;
        if (ImGui::Selectable(tag.Tag.c_str(), isSelected)) {
            m_SelectedEntity = ly::MakeRef<ly::scene::Entity>(entity, m_Scene.get());
        }
    }

    ImGui::EndChild();

    ImGui::SameLine();

    constexpr float componentsVerticalSplitRatio = 0.7f;
    ImGui::BeginChild("Entity", { windowSize.x * componentsVerticalSplitRatio, windowSize.y });
    if (m_SelectedEntity) {
        DrawComponent<ly::scene::TagComponent>("Tag");
        DrawComponent<ly::scene::IDComponent>("Id");
        DrawComponent<ly::scene::TransformComponent>("Transform");
    }
    ImGui::EndChild();

    ImGui::End();
}
