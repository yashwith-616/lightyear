#pragma once

#include "Lightyear.h"
#include "Sandbox/Editor/Workspace/ESceneWorkspace.h"

namespace ly::scene {
class Entity;
}

class EditorLayer : public ly::Layer {
public:
    EditorLayer();

    void onAttach() override;
    void onDetach() override;
    void onEvent(ly::Event& event) override {}
    void onEditorRender() override;
    void onUpdate(float deltaTime) override;

private:
    // TO BE REMOVED
    ly::ref<ly::scene::Scene> m_scene;
    ly::ref<ly::scene::SceneRuntime> m_sceneRuntime;

    // TO BE REMOVED
    ly::ref<ly::renderer::Framebuffer> m_framebuffer;
    ly::ref<ly::renderer::Shader> m_shader;
    ly::ref<ly::renderer::Texture2D> m_texture;

    // Will be present
    ly::scope<ESceneWorkspace> m_sceneWorkspace;
    ly::ref<GlobalEditorContext> m_editorContext;

    float m_prevMouseY{ 0.f };
    float m_prevMouseX{ 0.f };
    float m_mouseSensitivity{ 0.1f };

    void saveScene() const;
    void loadScene();
};