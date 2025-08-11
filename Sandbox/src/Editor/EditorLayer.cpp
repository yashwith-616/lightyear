#include "Sandbox/Editor/EditorLayer.h"
#include "Sandbox/Geometry/Geometry.h"
#include "Sandbox/Helpers/GridRender.h"

LY_DISABLE_WARNINGS_PUSH
#include <imgui.h>
#include <imgui_internal.h>
LY_DISABLE_WARNINGS_POP

constexpr glm::vec4 kClearColor = glm::vec4(0.12, 0.12, 0.13, 1.0);

namespace renderer = ly::renderer;

EditorLayer::EditorLayer() : Layer("Editor") {}

void EditorLayer::OnAttach() {
    m_Scene        = ly::MakeRef<ly::scene::Scene>();
    m_SceneRuntime = ly::MakeScope<ly::scene::SceneRuntime>(m_Scene.get());
    m_SceneRuntime->Initialize();

#pragma region Framebuffer Init
    // TODO: Framebuffer must be moved to a separate Framebuffer pool and must be accessed by the
    // world. The world needs to be responsible for rendering.

    renderer::FramebufferSpecification spec;
    spec.Width    = ly::kDefaultWindowSize.x;
    spec.Height   = ly::kDefaultWindowSize.y;
    m_Framebuffer = renderer::Framebuffer::Create(spec);
#pragma endregion

#pragma region Shaders and Textures
    // TODO: Texture and Shader will be made as part of the Entity.
    // Will be removed from here in the future implementation

    m_Texture = renderer::Texture2D::Create(ASSET_DIR "/Textures/T_Grid.png");
    m_Shader  = renderer::Shader::Create("ShaderBg", g_GridShader);
#pragma endregion

#pragma region SceneCamera
    ly::scene::Entity editorCamera = m_Scene->CreateEntity("EditorCamera");
    editorCamera.AddComponent<ly::scene::CameraComponent>();
    editorCamera.AddSingletonComponent<ly::scene::EditorCameraTag>();
#pragma endregion

#pragma region Game Scene
    ly::scene::Entity gameCamera = m_Scene->CreateEntity("GameCamera");
    gameCamera.AddComponent<ly::scene::CameraComponent>();
    gameCamera.AddSingletonComponent<ly::scene::MainCameraTag>();

    m_CubeEntity = m_Scene->CreateEntity("Cube");
    m_CubeEntity.AddComponent<ly::scene::MeshComponent>(Geometry::GetCube(), m_Shader, m_Texture);
    m_CubeEntity.AddComponent<ly::scene::RenderComponent>();

    m_SceneRuntime->OnViewportResize(glm::uvec2(spec.Width, spec.Height));
    m_SceneRuntime->SetSceneExecState(ly::scene::SceneRuntimeMode::PLAY);
#pragma endregion

#pragma region Inti Scene and Panels
    m_SceneWorkspace                    = ly::MakeScope<ESceneWorkspace>("SceneWorkspace");
    m_EditorContext                     = ly::MakeRef<GlobalEditorContext>();
    m_EditorContext->ActiveScene        = m_Scene;
    m_EditorContext->SceneFramebufferId = m_Framebuffer->GetColorAttachmentRenderID();
    m_SceneWorkspace->OnAttach(m_EditorContext);
#pragma endregion
}

void EditorLayer::OnUpdate(float deltaTime) {
    PollInput(deltaTime);
    m_Framebuffer->Bind();
    renderer::RenderCommand::Clear();
    renderer::RenderCommand::SetClearColor(kClearColor);

    // m_SceneRuntime->OnStart();
    m_SceneRuntime->OnUpdate(deltaTime);
    // m_SceneRuntime->OnEnd();

    m_Framebuffer->Unbind();
}

void EditorLayer::OnEditorRender() {
    m_SceneWorkspace->OnEditorUpdate();
    m_SceneWorkspace->OnImGuiRender();

    ImGui::SetNextWindowBgAlpha(0.35f);  // Transparent background
    if (ImGui::Begin("Example: Simple overlay")) {
        ImGui::Text(std::format("IsRunning: {}", m_SceneRuntime->IsRunning()).c_str());
        ImGui::Text(std::format("IsPaused: {}", m_SceneRuntime->IsPaused()).c_str());
    }
    ImGui::End();
}

/**
 * @brief Move the camera based on the following action
 * @param deltaTime the deltaTime
 */
void EditorLayer::PollInput(float deltaTime) {
    if (ly::Input::IsKeyPressed(ly::Key::Q)) {
        // m_EditorCamera->MoveUp(deltaTime * m_EditorCamera->GetSpeed());
    }

    if (ly::Input::IsKeyPressed(ly::Key::E)) {
        // m_EditorCamera->MoveUp(deltaTime * m_EditorCamera->GetSpeed() * -1.f);
    }

    if (ly::Input::IsKeyPressed(ly::Key::D)) {
        // m_EditorCamera->MoveRight(deltaTime * m_EditorCamera->GetSpeed());
    }

    if (ly::Input::IsKeyPressed(ly::Key::A)) {
        // m_EditorCamera->MoveRight(deltaTime * m_EditorCamera->GetSpeed() * -1.f);
    }

    if (ly::Input::IsKeyPressed(ly::Key::W)) {
        // m_EditorCamera->MoveForward(deltaTime * m_EditorCamera->GetSpeed() * -1.f);
    }

    if (ly::Input::IsKeyPressed(ly::Key::S)) {
        // m_EditorCamera->MoveForward(deltaTime * m_EditorCamera->GetSpeed());
    }

    if (ly::Input::IsMouseButtonPressed(ly::Mouse::Button1)) {
        if (const float val = ly::Input::GetMouseY(); val > 0) {
            const float diff = val - m_PrevMouseY;
            // m_EditorCamera->AddPitch(diff * deltaTime * m_MouseSensitivity);
            m_PrevMouseY = val;
        }

        if (const float val = ly::Input::GetMouseX(); val > 0) {
            const float diff = val - m_PrevMouseX;
            // m_EditorCamera->AddYaw(diff * deltaTime * m_MouseSensitivity);
            m_PrevMouseX = val;
        }
    }
}
