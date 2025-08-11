#pragma once

#include "Sandbox/Editor/Panel/IEditorPanel.h"

class EViewportPanel : public IEditorPanel {
public:
    explicit EViewportPanel(std::string name) : IEditorPanel(std::move(name)) {}
    ~EViewportPanel() override = default;

    void OnImGuiRender() override;

    void SetFramebuffer(uint32_t framebufferId) { m_FramebufferId = framebufferId; }

private:
    uint32_t m_FramebufferId { 0 };
};