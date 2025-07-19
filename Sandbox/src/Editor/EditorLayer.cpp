#include "Sandbox/Editor/EditorLayer.h"
#include "Sandbox/Core/Camera/EditorCamera.h"
#include "Sandbox/Geometry/Geometry.h"
#include "Sandbox/Helpers/GridRender.h"

LY_DISABLE_WARNINGS_PUSH
#include <imgui.h>
#include <imgui_internal.h>
LY_DISABLE_WARNINGS_POP

namespace renderer = ly::renderer;

EditorLayer::EditorLayer() : Layer("Editor") {}

void EditorLayer::OnAttach() {
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
    // TODO: Move to separate functions to create a SceneCamera
    // Camera system will be re-written using entt, current implementation
    // will be modified significantly

    m_EditorCamera = ly::MakeRef<EditorCamera>(ly::kDefaultAspectRatio);
    m_EditorCamera->SetSpeed(1.f);
#pragma endregion

#pragma region Game Scene
    m_Scene        = ly::MakeRef<ly::scene::Scene>();
    m_SceneRuntime = ly::MakeScope<ly::scene::SceneRuntime>(m_Scene.get());

    auto newCamera                 = ly::MakeRef<EditorCamera>(ly::kDefaultAspectRatio);
    ly::scene::Entity cameraEntity = m_Scene->CreateEntity("GameCamera");
    cameraEntity.AddComponent<ly::scene::CameraComponent>(newCamera);
    m_SceneRuntime->OnViewportResize(glm::uvec2(spec.Width, spec.Height));

    m_CubeEntity = m_Scene->CreateEntity("Cube");
    m_CubeEntity.AddComponent<ly::scene::MeshComponent>(Geometry::GetCube(), m_Shader, m_Texture);
    m_CubeEntity.AddComponent<ly::scene::RenderComponent>();
#pragma endregion

#pragma region Inti Scene and Panels
    m_SceneWorkspace               = ly::MakeScope<ESceneWorkspace>("SceneWorkspace");
    m_EditorContext                = ly::MakeRef<GlobalEditorContext>();
    m_EditorContext->m_ActiveScene = m_Scene;
    m_SceneWorkspace->OnAttach(m_EditorContext);
#pragma endregion
}

void EditorLayer::OnUpdate(float deltaTime) {
    PollInput(deltaTime);

    constexpr glm::vec4 clearColor(0.0f, 0.0f, 0.0f, 0.5f);

    m_Framebuffer->Bind();
    renderer::RenderCommand::Clear();

    if (m_SceneRuntime->IsPaused()) {
        m_SceneRuntime->OnUpdateEditor(deltaTime, std::static_pointer_cast<ly::renderer::SceneCamera>(m_EditorCamera));
    }

    if (m_SceneRuntime->IsRunning()) {
        m_SceneRuntime->OnUpdateRuntime(deltaTime);
    }

    m_Framebuffer->Unbind();
}

void EditorLayer::OnEditorRender() {
    m_SceneWorkspace->OnEditorUpdate();
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
        if (const float val = ly::Input::GetMouseY(); val > 0) {
            const float diff = val - m_PrevMouseY;
            m_EditorCamera->AddPitch(diff * deltaTime * m_MouseSensitivity);
            m_PrevMouseY = val;
        }

        if (const float val = ly::Input::GetMouseX(); val > 0) {
            const float diff = val - m_PrevMouseX;
            m_EditorCamera->AddYaw(diff * deltaTime * m_MouseSensitivity);
            m_PrevMouseX = val;
        }
    }
}
