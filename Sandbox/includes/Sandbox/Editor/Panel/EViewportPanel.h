#pragma once

#include "Lightyear.h"
#include "Sandbox/Editor/Panel/IEditorPanel.h"

class EViewportPanel : public IEditorPanel {
public:
    explicit EViewportPanel(std::string name) : IEditorPanel(std::move(name)) {}
    ~EViewportPanel() override = default;

    void OnImGuiRender() override;

    void SetFramebuffer(ly::Ref<ly::renderer::Framebuffer> framebuffer) { m_Framebuffer = framebuffer; }

    void SetSceneRuntime(ly::Ref<ly::scene::SceneRuntime> sceneRuntime) { m_SceneRuntime = sceneRuntime; }

private:
    ly::Ref<ly::renderer::Framebuffer> m_Framebuffer;
    ly::Ref<ly::scene::SceneRuntime> m_SceneRuntime;
    ImVec2 m_ViewportSize{};
};