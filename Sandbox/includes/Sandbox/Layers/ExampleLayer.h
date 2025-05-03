#pragma once

#include "Lightyear.h"

class EditorCamera;

class ExampleLayer : public ly::Layer {
public:
    ExampleLayer();
    void OnUpdate(float deltaTime) override;
    void OnEvent(ly::Event& event) override;

private:
    void PollInput(float deltaTime);

private:
    ly::Ref<ly::renderer::Shader> m_Shader;
    ly::Ref<ly::renderer::Shader> m_Shader2;

    ly::Ref<EditorCamera> m_Camera;
};
