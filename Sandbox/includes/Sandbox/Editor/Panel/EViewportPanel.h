#pragma once

#include "Lightyear.h"
#include "Sandbox/Editor/Panel/IEditorPanel.h"

class EViewportPanel : public IEditorPanel {
public:
    explicit EViewportPanel(std::string name) : IEditorPanel(std::move(name)) {}
    ~EViewportPanel() override = default;

    void onImGuiRender() override;

    void setFramebuffer(ly::ref<ly::renderer::Framebuffer> framebuffer) { m_framebuffer = framebuffer; }

    void setSceneRuntime(ly::ref<ly::scene::SceneRuntime> sceneRuntime) { m_sceneRuntime = sceneRuntime; }

private:
    ly::ref<ly::renderer::Framebuffer> m_framebuffer;
    ly::ref<ly::scene::SceneRuntime> m_sceneRuntime;
    ImVec2 m_viewportSize{};
};