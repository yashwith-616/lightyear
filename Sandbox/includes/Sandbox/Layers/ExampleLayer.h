#pragma once

#include "Lightyear.h"

class ExampleLayer : public ly::Layer {
public:
    ExampleLayer();
    void OnUpdate() override;
    void OnEvent(ly::Event& event) override;

private:
    ly::renderer::RendererAPI * m_RenderAPI;
    ly::renderer::Shader* m_Shader;
    ly::renderer::Shader* m_Shader2;
};
