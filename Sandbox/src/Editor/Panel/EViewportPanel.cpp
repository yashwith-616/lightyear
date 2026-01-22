#include "Sandbox/Editor/Panel/EViewportPanel.h"

/*
 * TODO: Handle custom resizing for Screen
 */
void EViewportPanel::onImGuiRender()
{
    ImGui::Begin("Viewport");
    ImGui::BeginChild("Game Render", ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollbar);

    ImVec2 const newViewportSize = ImGui::GetContentRegionAvail();
    if (newViewportSize.x > 0 && newViewportSize.y > 0)
    {
        bool const isViewportSizeChanged =
            m_viewportSize.x != newViewportSize.x || m_viewportSize.y != newViewportSize.y;
        if (isViewportSizeChanged)
        {
            m_viewportSize = newViewportSize;
            auto const width = ly::narrowCast<uint32_t>(m_viewportSize.x);
            auto const height = ly::narrowCast<uint32_t>(m_viewportSize.y);
            m_framebuffer->resize(width, height);

            // TODO: Dispatch an event instead
            m_sceneRuntime->onViewportResize(
                {ly::narrowCast<uint32_t>(newViewportSize.x), ly::narrowCast<uint32_t>(newViewportSize.y)});
        }

        // Draw framebuffer texture
        ImGui::Image(
            m_framebuffer->getColorAttachmentRenderId(),
            m_viewportSize,
            ImVec2(0, 1), // UV top-left
            ImVec2(1, 0)  // UV bottom-right
        );
    }

    ImGui::EndChild();
    ImGui::End();
}
