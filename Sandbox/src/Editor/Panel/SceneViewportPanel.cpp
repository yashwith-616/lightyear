#include "Sandbox/Editor/Panel/SceneViewportPanel.h"

SceneViewportPanel::~SceneViewportPanel() {}

void SceneViewportPanel::OnImGuiRender() {
    ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_MenuBar);
    // === Top Menu Bar (for play/pause button) ===
    if (ImGui::BeginMenuBar()) {
        if (m_ActiveScene->IsRunning()) {
            if (ImGui::Button("⏸ Pause")) {
                m_ActiveScene->SetSceneExecState(ly::scene::SS_PAUSED);
                LY_LOG(ly::LogType::Info, "Scene paused");
            }
        } else {
            if (ImGui::Button("▶ Play")) {
                m_ActiveScene->SetSceneExecState(ly::scene::SS_RUNNING);
                LY_LOG(ly::LogType::Info, "Scene running");
            }
        }

        float toolbarWidth = ImGui::GetContentRegionAvail().x;
        ImGui::SameLine(toolbarWidth - 120);
        if (ImGui::Button("CameraSettings")) {
            m_ShowCameraSettings = !m_ShowCameraSettings;
        }

        DrawCameraToolbar();
        ImGui::EndMenuBar();
    }

    /// === Viewport resizing ===
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    if ((uint32_t)viewportSize.x != m_ActiveFramebuffer->GetSpecification().Width ||
        (uint32_t)viewportSize.y != m_ActiveFramebuffer->GetSpecification().Height) {
        m_ActiveFramebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
        m_ActiveScene->OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
        LY_LOG(
            ly::LogType::Info, "Viewport size updated: {0}, {1}", viewportSize.x, viewportSize.y);
    }

    // === Viewport rendering ===
    ImGui::Image(m_ActiveFramebuffer->GetColorAttachmentRenderID(),
                 viewportSize,
                 ImVec2(0, 1),
                 ImVec2(1, 0));

    ImGui::End();
}

void SceneViewportPanel::DrawCameraToolbar() {
    if (!m_ShowCameraSettings) return;

    ImGui::Begin("Camera Settings", &m_ShowCameraSettings);

    float speed = m_SceneCamera->GetSpeed();
    if (ImGui::SliderFloat("Camera Speed", &speed, 0.0f, 8.0f)) {
        m_SceneCamera->SetSpeed(speed);
    }

    if (ImGui::SliderFloat("Camera Zoom", &m_CameraZoom, 0.1f, 8.0f)) {
        // m_SceneCamera->SetZoom(m_CameraZoom);
    }

    if (ImGui::Checkbox("Perspective Camera", &m_IsPerspectiveCamera)) {
        if (m_IsPerspectiveCamera)
            m_SceneCamera->SetPerspective(glm::radians(90.0f), 1.7777778f, 0.1f, 1000.0f);
        else
            m_SceneCamera->SetOrthographic(
                10.0f, 1.7777778f, 0.1f, 1000.0f);  // 10.0f is ortho size
    }

    if (ImGui::Button("Reset to Default")) {
        m_SceneCamera->SetSpeed(1.0f);
        m_CameraZoom = 1.0f;
        // m_SceneCamera->SetZoom(m_CameraZoom);
    }

    ImGui::End();
}