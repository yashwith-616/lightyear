#pragma once

#include "Lightyear.h"

namespace renderer = ly::renderer;
class ExampleLayer : public ly::Layer {
public:
    ExampleLayer();
    void OnUpdate() override;
    void OnEvent(ly::Event& event) override;

private:
    renderer::RendererAPI* m_RenderAPI;
    renderer::Shader* m_Shader;
    renderer::Shader* m_Shader2;
};
