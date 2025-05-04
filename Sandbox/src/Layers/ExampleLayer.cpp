#include "Sandbox/Layers/ExampleLayer.h"
#include "Sandbox/Core/Camera/EditorCamera.h"
#include "Sandbox/Geometry/Geometry.h"
#include "glm/glm.hpp"

namespace renderer = ly::renderer;

ExampleLayer::ExampleLayer() : Layer("Example") {
    std::unordered_map<renderer::ShaderType, ly::CPath> shader1 = {
        { renderer::ShaderType::Vertex, ASSET_DIR "/Shaders/Vertex/default.vert" },
        { renderer::ShaderType::Fragment, ASSET_DIR "/Shaders/Fragment/default.frag" }
    };

    std::unordered_map<renderer::ShaderType, ly::CPath> shader2 = {
        { renderer::ShaderType::Vertex, ASSET_DIR "/Shaders/Vertex/default2.vert" },
        { renderer::ShaderType::Fragment, ASSET_DIR "/Shaders/Fragment/default2.frag" }
    };

    float aspect = 1280.f / 720.f;
    m_Camera     = ly::MakeRef<EditorCamera>(-aspect, aspect, -1.f, 1.f);
}

void ExampleLayer::OnUpdate(float deltaTime) {
    PollInput(deltaTime);

    renderer::RenderCommand::SetClearColor(glm::vec4(0.f, 0.f, 0.f, 0.5f));
    renderer::RenderCommand::Clear();

    renderer::Renderer::BeginScene(m_Camera);

    renderer::Renderer::Submit(m_Shader, Geometry::GetCube(), glm::mat4(1.f));
    renderer::Renderer::Submit(m_Shader2, Geometry::GetPlane(), glm::mat4(1.f));

    renderer::Renderer::EndScene();
}

void ExampleLayer::OnEvent(ly::Event& event) {}

/**
 * @brief Move the camera based on the following action
 * @param deltaTime the deltaTime
 */
void ExampleLayer::PollInput(float deltaTime) {
    if (ly::Input::IsKeyPressed(ly::Key::Q)) {
        m_Camera->DebugCamera();
        m_Camera->MoveUp(deltaTime * m_Camera->GetSpeed());
    }

    if (ly::Input::IsKeyPressed(ly::Key::E)) {
        m_Camera->DebugCamera();
        m_Camera->MoveUp(deltaTime * m_Camera->GetSpeed() * -1.f);
    }

    if (ly::Input::IsKeyPressed(ly::Key::D)) {
        m_Camera->DebugCamera();
        m_Camera->MoveRight(deltaTime * m_Camera->GetSpeed());
    }

    if (ly::Input::IsKeyPressed(ly::Key::A)) {
        m_Camera->DebugCamera();
        m_Camera->MoveRight(deltaTime * m_Camera->GetSpeed() * -1.f);
    }

    if (ly::Input::IsKeyPressed(ly::Key::W)) {
        m_Camera->DebugCamera();
        m_Camera->MoveForward(deltaTime * m_Camera->GetSpeed());
    }

    if (ly::Input::IsKeyPressed(ly::Key::S)) {
        m_Camera->DebugCamera();
        m_Camera->MoveForward(deltaTime * m_Camera->GetSpeed() * -1.f);
    }

    if (ly::Input::IsKeyPressed(ly::Key::J)) {
        m_Camera->SetSpeed(m_Camera->GetSpeed() + 50000);
    }

    if (ly::Input::IsKeyPressed(ly::Key::K)) {
        m_Camera->SetSpeed(m_Camera->GetSpeed() - 50000);
    }
}