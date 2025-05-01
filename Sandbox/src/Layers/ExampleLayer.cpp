#include "Sandbox/Layers/ExampleLayer.h"
#include "Sandbox/Geometry/Geometry.h"
#include "glm/glm.hpp"

ExampleLayer::ExampleLayer() : Layer("Example") {
    m_RenderAPI = renderer::RendererAPI::Create();
    m_Shader =
        renderer::Shader::Create("F:/workspace/cpp/LightYearWorkspace/Sandbox/assets/Shaders/Vertex/default.vert",
                                 "F:/workspace/cpp/LightYearWorkspace/Sandbox/assets/Shaders/Fragment/default.frag");

    m_Shader2 =
        renderer::Shader::Create("F:/workspace/cpp/LightYearWorkspace/Sandbox/assets/Shaders/Vertex/default2.vert",
                                 "F:/workspace/cpp/LightYearWorkspace/Sandbox/assets/Shaders/Fragment/default2.frag");
}

void ExampleLayer::OnUpdate() {
    m_RenderAPI->Init();
    m_RenderAPI->SetClearColor(glm::vec4(0.f, 0.f, 0.f, 0.3f));
    m_RenderAPI->Clear();

    m_Shader->Use();
    Geometry::DrawPlane(m_RenderAPI);

    m_Shader2->Use();
    Geometry::DrawCube(m_RenderAPI);
}

void ExampleLayer::OnEvent(ly::Event& event) {}