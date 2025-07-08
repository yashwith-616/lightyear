#pragma once

#include "Lightyear.h"
#include "Panel/SceneGraphPanel.h"
#include "Sandbox/Editor/Workspace/ESceneWorkspace.h"

class EditorCamera;

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

private:
    // TO BE REMOVED
    ly::Ref<ly::scene::Scene> m_Scene{};

    // TO BE REMOVED
    ly::Ref<ly::renderer::Framebuffer> m_Framebuffer{};
    ly::Ref<ly::renderer::Shader> m_Shader;
    ly::Ref<ly::renderer::Texture2D> m_Texture;
    ly::Ref<EditorCamera> m_EditorCamera{};
    
    ly::scene::Entity m_CubeEntity{};

    // Will be present
    ly::Scope<ESceneWorkspace> m_SceneWorkspace{};
    ly::Ref<GlobalEditorContext> m_EditorContext{};

private:
    float m_PrevMouseY{ 0.f };
    float m_PrevMouseX{ 0.f };
    float m_MouseSensitivity{ 0.1f };

    void PollInput(float deltaTime);
};