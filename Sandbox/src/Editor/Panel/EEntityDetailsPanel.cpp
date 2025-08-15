#include "Sandbox/Editor/Panel/EEntityDetailsPanel.h"

void EEntityDetailsPanel::OnImGuiRender() {
    ImGui::Begin("Entity Component Details");
    if (m_SelectedEntity == nullptr) {
        ImGui::End();
        return;
    }
    DrawAllComponents<ly::scene::AllComponents>(*m_SelectedEntity);
    ImGui::End();
}
