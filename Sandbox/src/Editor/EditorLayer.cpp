#include "Sandbox/Editor/EditorLayer.h"
#include "Sandbox/Core/Camera/EditorCamera.h"
#include "Sandbox/Geometry/Geometry.h"
#include "Sandbox/Helpers/GridRender.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace renderer = ly::renderer;

// TODO: Remove
static constexpr float aspect = 1.77778;

// TODO: Remove
const std::unordered_map<renderer::ShaderType, ly::CPath> GRID_SHADER = {
    { renderer::ShaderType::Vertex, ASSET_DIR "/Shaders/Vertex/S_Default.vert" },
    { renderer::ShaderType::Fragment, ASSET_DIR "/Shaders/Fragment/S_Default.frag" }
};

EditorLayer::EditorLayer() : ly::Layer("Editor") {}

void EditorLayer::OnAttach() {
#pragma region Framebuffer Init
    // TODO: Framebuffer must be moved to a separate Framebuffer pool and must be accessed by the
    // world. The world needs to be responsible for rendering.

    ly::renderer::FramebufferSpecification spec;
    spec.Height   = 1280;
    spec.Width    = 720;
    m_Framebuffer = ly::renderer::Framebuffer::Create(spec);
#pragma endregion

#pragma region Shaders and Textures
    // TODO: Texture and Shader will be made as part of the Entity.
    // Will be removed from here in the future implementation

    m_Texture = ly::renderer::Texture2D::Create(ASSET_DIR "/Textures/T_Grid.png");
    m_Shader  = ly::renderer::Shader::Create("ShaderBg", GRID_SHADER);
#pragma endregion

#pragma region SceneCamera
    // TODO: Move to separate functions to create a SceneCamera
    // Camera system will be re-written using entt, current implementation
    // will be modified significantly

    m_EditorCamera = ly::MakeRef<EditorCamera>(aspect);
    m_EditorCamera->SetSpeed(1.f);
#pragma endregion

#pragma region Game Scene
    m_Scene                        = ly::MakeRef<ly::scene::Scene>();
    auto newCamera                 = ly::MakeRef<EditorCamera>(aspect);
    ly::scene::Entity cameraEntity = m_Scene->CreateEntity("GameCamera");
    cameraEntity.AddComponent<ly::scene::CameraComponent>(newCamera);
    m_Scene->OnViewportResize(1280, 720);

    m_CubeEntity = m_Scene->CreateEntity("Cube");
    m_CubeEntity.AddComponent<ly::scene::MeshComponent>(Geometry::GetCube(), m_Shader, m_Texture);
    m_CubeEntity.AddComponent<ly::scene::RenderComponent>();

    ly::scene::TransformComponent cubeTransform = m_CubeEntity.GetComponent<ly::scene::TransformComponent>();
    cubeTransform.GetTransform();
#pragma endregion

#pragma region Inti Scene and Panels
    m_SceneWorkspace               = ly::MakeScope<ESceneWorkspace>("SceneWorkspace");
    m_EditorContext                = ly::MakeRef<GlobalEditorContext>();
    m_EditorContext->m_ActiveScene = m_Scene;
    m_SceneWorkspace->OnAttach(m_EditorContext);
#pragma endregion
}

void EditorLayer::OnEvent(ly::Event& event) {}

void EditorLayer::OnUpdate(float deltaTime) {
    PollInput(deltaTime);

    m_Framebuffer->Bind();
    renderer::RenderCommand::SetClearColor(glm::vec4(0.f, 0.f, 0.f, 0.5f));
    renderer::RenderCommand::Clear();

    if (m_Scene->IsPaused()) {
        m_Scene->OnUpdateEditor(deltaTime, std::static_pointer_cast<ly::renderer::SceneCamera>(m_EditorCamera));
    }

    if (m_Scene->IsRunning()) {
        m_Scene->OnUpdateRuntime(deltaTime);
    }

    m_Framebuffer->Unbind();
}

void EditorLayer::OnEditorRender() {
    m_SceneWorkspace->OnEditorUpdate();
    // DrawDockspace();

    // ImGui::ShowDemoWindow();
    m_SceneWorkspace->OnImGuiRender();
}

/**
 * @brief Move the camera based on the following action
 * @param deltaTime the deltaTime
 */
void EditorLayer::PollInput(float deltaTime) {
    if (ly::Input::IsKeyPressed(ly::Key::Q)) {
        m_EditorCamera->MoveUp(deltaTime * m_EditorCamera->GetSpeed());
    }

    if (ly::Input::IsKeyPressed(ly::Key::E)) {
        m_EditorCamera->MoveUp(deltaTime * m_EditorCamera->GetSpeed() * -1.f);
    }

    if (ly::Input::IsKeyPressed(ly::Key::D)) {
        m_EditorCamera->MoveRight(deltaTime * m_EditorCamera->GetSpeed());
    }

    if (ly::Input::IsKeyPressed(ly::Key::A)) {
        m_EditorCamera->MoveRight(deltaTime * m_EditorCamera->GetSpeed() * -1.f);
    }

    if (ly::Input::IsKeyPressed(ly::Key::W)) {
        m_EditorCamera->MoveForward(deltaTime * m_EditorCamera->GetSpeed() * -1.f);
    }

    if (ly::Input::IsKeyPressed(ly::Key::S)) {
        m_EditorCamera->MoveForward(deltaTime * m_EditorCamera->GetSpeed());
    }

    if (ly::Input::IsMouseButtonPressed(ly::Mouse::Button1)) {
        if (const float val = ly::Input::GetMouseY()) {
            const float diff = val - m_PrevMouseY;
            m_EditorCamera->AddPitch(diff * deltaTime * m_MouseSensitivity);
            m_PrevMouseY = val;
        }

        if (const float val = ly::Input::GetMouseX()) {
            const float diff = val - m_PrevMouseX;
            m_EditorCamera->AddYaw(diff * deltaTime * m_MouseSensitivity);
            m_PrevMouseX = val;
        }
    }
}
