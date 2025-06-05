#pragma once

#include "IEditorPanel.h"
#include "Sandbox/Core/Camera/EditorCamera.h"

class SceneViewportPanel : public IEditorPanel {
private:
    enum class PlayState { PLAY, PAUSE, SIMULATE, NONE };

public:
    ~SceneViewportPanel();

    void OnImGuiRender();

    inline void SetActiveScene(ly::Ref<ly::scene::Scene> scene) { m_ActiveScene = scene; }

    inline void SetActiveCamera(ly::Ref<EditorCamera> camera) { m_SceneCamera = camera; }

    inline void SetFramebuffer(ly::Ref<ly::renderer::Framebuffer> framebuffer) {
        m_ActiveFramebuffer = framebuffer;
    }

private:
    ly::Ref<ly::scene::Scene> m_ActiveScene{};
    ly::Ref<EditorCamera> m_SceneCamera{};
    ly::Ref<ly::renderer::Framebuffer> m_ActiveFramebuffer{};

    float m_CameraSpeed{ 1.f };
    float m_CameraZoom{ 1.f };
    bool m_ShowCameraSettings{ false };
    bool m_IsPerspectiveCamera{ false };

    PlayState m_PlayState = PlayState::NONE;

private:
    void DrawCameraToolbar();
};
