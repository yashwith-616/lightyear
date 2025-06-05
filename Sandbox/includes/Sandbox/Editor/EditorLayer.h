#pragma once

#include "Lightyear.h"
#include "Panel/SceneViewportPanel.h"

class EditorCamera;
class GridRender;

namespace ly::scene {
class Entity;
}

class EditorLayer : public ly::Layer {
public:
    EditorLayer();

    virtual void OnAttach() override;
    virtual void OnEvent(ly::Event& event) override;
    virtual void OnEditorRender() override;
    virtual void OnUpdate(float deltaTime) override;

protected:
    virtual void InitEditorLayouts();

private:
    ly::Ref<ly::renderer::Framebuffer> m_Framebuffer{};
    ly::Ref<ly::renderer::Shader> m_Shader;
    ly::Ref<ly::renderer::Texture2D> m_Texture;
    ly::Ref<ly::scene::Scene> m_Scene{};

    ly::Ref<EditorCamera> m_EditorCamera{};
    ly::Scope<GridRender> m_GridRenderer{};

    SceneViewportPanel m_ViewportPanel{};

    ly::scene::Entity m_CubeEntity{};

private:
    float m_PrevMouseY{ 0.f };
    float m_PrevMouseX{ 0.f };
    float m_MouseSensitivity{ 0.1f };

private:
    void PollInput(float deltaTime);

    void DrawDockspace();
    void DrawDemoSettings();
    void DrawLogPanel();
    void DrawTestPanel();
};