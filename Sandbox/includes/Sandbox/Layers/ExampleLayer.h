#pragma once

#include "Lightyear.h"

class EditorCamera;

class ExampleLayer : public ly::Layer {
public:
    ExampleLayer();
    virtual void OnUpdate(float deltaTime) override;
    virtual void OnEvent(ly::Event& event) override;
    virtual void OnEditorRender() override;

private:
    void PollInput(float deltaTime);

private:
    ly::Ref<ly::renderer::Shader> m_Shader;
    ly::Ref<ly::renderer::Shader> m_Shader2;

    ly::Ref<EditorCamera> m_Camera;
};
