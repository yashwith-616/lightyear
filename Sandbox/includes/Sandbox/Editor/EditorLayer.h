#pragma once

#include "Lightyear.h"
#include "Sandbox/Editor/Workspace/ESceneWorkspace.h"

namespace ly::scene
{
class Entity;
}

class EditorLayer : public ly::Layer
{
public:
    EditorLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnEvent(ly::Event& event) override {}
    void OnEditorRender() override;
    void OnUpdate(float deltaTime) override;

private:
    // TO BE REMOVED
    ly::Ref<ly::scene::Scene> m_Scene;
    ly::Ref<ly::scene::SceneRuntime> m_SceneRuntime;

    // TO BE REMOVED
    ly::Ref<ly::renderer::Framebuffer> m_Framebuffer;
    ly::Ref<ly::renderer::Shader> m_Shader;
    ly::Ref<ly::renderer::Texture2D> m_Texture;

    // Will be present
    ly::Scope<ESceneWorkspace> m_SceneWorkspace;
    ly::Ref<GlobalEditorContext> m_EditorContext;

    float m_PrevMouseY{0.f};
    float m_PrevMouseX{0.f};
    float m_MouseSensitivity{0.1f};

    void SaveScene() const;
    void LoadScene();
};
