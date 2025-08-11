#include "Sandbox/Editor/Panel/EViewportPanel.h"

void EViewportPanel::OnImGuiRender() {
    ImGui::Begin("Viewport");

    ImGui::BeginChild("Game Render", ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollbar);
    {
        const ImVec2 availSize = ImGui::GetContentRegionAvail();
        ImGui::Image(m_FramebufferId, availSize, ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::EndChild();

    ImGui::End();
}