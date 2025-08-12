#include "Sandbox/Editor/Panel/EEntityDetailsPanel.h"

void EEntityDetailsPanel::OnImGuiRender() {
    ImGui::Begin("Entity Component Details");
    if (m_SelectedEntity != nullptr) {
        if (m_SelectedEntity->HasComponent<ly::scene::TagComponent>()) {
            DrawComponent<ly::scene::TagComponent>();
        }

        if (m_SelectedEntity->HasComponent<ly::scene::IDComponent>()) {
            DrawComponent<ly::scene::IDComponent>();
        }

        if (m_SelectedEntity->HasComponent<ly::scene::TransformComponent>()) {
            DrawComponent<ly::scene::TransformComponent>();
        }
    }
    ImGui::End();
}
