#include "Sandbox/Editor/Panel/EEntityDetailsPanel.h"

void EEntityDetailsPanel::onImGuiRender() {
    ImGui::Begin("Entity Component Details");
    if (m_selectedEntity == nullptr) {
        ImGui::End();
        return;
    }
    addComponent();
    drawComponentPanel();
    ImGui::End();
}

void EEntityDetailsPanel::addComponent() const {
    ImGui::BeginChild("Add Component", ImVec2(0, 30), false);
    if (ImGui::Button("Add Component", ImVec2(-FLT_MIN, 0))) {
        m_selectedEntity->addComponent<ly::scene::MeshComponent>();
    }
    ImGui::EndChild();
}

void EEntityDetailsPanel::drawComponentPanel() {
    ImGui::BeginChild("Components", ImVec2(0, 0), true);
    drawAllComponents<ly::scene::allComponents>(*m_selectedEntity);
    ImGui::EndChild();
}
