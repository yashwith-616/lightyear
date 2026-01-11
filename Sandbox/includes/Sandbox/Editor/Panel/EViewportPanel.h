#pragma once

#include "Lightyear.h"
#include "Sandbox/Editor/Panel/IEditorPanel.h"

class EViewportPanel : public IEditorPanel
{
public:
    explicit EViewportPanel(std::string name) : IEditorPanel(std::move(name)) {}
    ~EViewportPanel() override = default;

    void OnImGuiRender() override;

    void setFramebuffer(ly::Ref<ly::renderer::Framebuffer> const& framebuffer) { m_framebuffer = framebuffer; }

    void setSceneRuntime(ly::Ref<ly::scene::SceneRuntime> const& sceneRuntime) { m_sceneRuntime = sceneRuntime; }

private:
    ly::Ref<ly::renderer::Framebuffer> m_framebuffer;
    ly::Ref<ly::scene::SceneRuntime> m_sceneRuntime;
    ImVec2 m_viewportSize;
};
