#include "Sandbox/Editor/Panel/EEntityDetailsPanel.h"

void EEntityDetailsPanel::OnImGuiRender()
{
    ImGui::Begin("Entity Component Details");
    if (m_SelectedEntity == nullptr)
    {
        ImGui::End();
        return;
    }
    AddComponent();
    DrawComponentPanel();
    ImGui::End();
}

void EEntityDetailsPanel::DrawComponentPanel()
{
    ImGui::BeginChild("Components", ImVec2(0, 0), true);
    DrawAllComponents<ly::scene::AllComponents>(*m_SelectedEntity);
    ImGui::EndChild();
}

void EEntityDetailsPanel::AddComponent() const
{
    ImGui::BeginChild("Add Component", ImVec2(0, 30), false);
    if (ImGui::Button("Add Component", ImVec2(-FLT_MIN, 0)))
        m_SelectedEntity->AddComponent<ly::scene::MeshComponent>();
    ImGui::EndChild();
}
