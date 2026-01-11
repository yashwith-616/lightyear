#include "Sandbox/Editor/Panel/EViewportPanel.h"

/*
 * TODO: Handle custom resizing for Screen
 */
void EViewportPanel::OnImGuiRender()
{
    ImGui::Begin("Viewport");
    ImGui::BeginChild("Game Render", ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollbar);

    const ImVec2 newViewportSize = ImGui::GetContentRegionAvail();
    if (newViewportSize.x > 0 && newViewportSize.y > 0)
    {
        const bool isViewportSizeChanged =
            m_viewportSize.x != newViewportSize.x || m_viewportSize.y != newViewportSize.y;
        if (isViewportSizeChanged)
        {
            m_viewportSize = newViewportSize;
            const auto width = ly::narrow_cast<uint32_t>(m_viewportSize.x);
            const auto height = ly::narrow_cast<uint32_t>(m_viewportSize.y);
            m_framebuffer->Resize(width, height);

            // TODO: Dispatch an event instead
            m_sceneRuntime->OnViewportResize(
                {ly::narrow_cast<uint32_t>(newViewportSize.x), ly::narrow_cast<uint32_t>(newViewportSize.y)});
        }

        // Draw framebuffer texture
        ImGui::Image(
            m_framebuffer->GetColorAttachmentRenderID(),
            m_viewportSize,
            ImVec2(0, 1), // UV top-left
            ImVec2(1, 0)  // UV bottom-right
        );
    }

    ImGui::EndChild();
    ImGui::End();
}
