#include "Sandbox/Editor/EditorLayer.h"
#include "Sandbox/Geometry/Geometry.h"
#include "Sandbox/Helpers/GridRender.h"

LY_DISABLE_WARNINGS_PUSH
#include <imgui.h>
#include <imgui_internal.h>
LY_DISABLE_WARNINGS_POP

constexpr auto g_ClearColor = glm::vec4(0.13, 0.13, 0.13, 1.0);

namespace renderer = ly::renderer;

EditorLayer::EditorLayer() : Layer("Editor") {}

void EditorLayer::OnAttach() {
    m_Scene        = ly::MakeRef<ly::scene::Scene>();
    m_SceneRuntime = ly::MakeRef<ly::scene::SceneRuntime>(m_Scene.get());
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
    m_Shader  = renderer::Shader::Create("ShaderBg", g_DefaultShader);
#pragma endregion

#pragma region Object2
    /*auto planeEntity = m_Scene->CreateEntity("PlaneObject");
    planeEntity.AddComponent<ly::scene::MeshComponent>(
        Geometry::GetPlane(), renderer::Shader::Create("GridShader", g_GridShader), m_Texture);
    planeEntity.AddComponent<ly::scene::RenderComponent>();*/

#pragma endregion

#pragma region SceneCamera
    ly::scene::Entity editorCamera = m_Scene->CreateEntity("EditorCamera");
    editorCamera.AddComponent<ly::scene::CameraComponent>();
    editorCamera.AddSingletonComponent<ly::scene::EditorCameraTag>();
#pragma endregion

#pragma region Game Scene
    auto gameCamera = m_Scene->CreateEntity("GameCamera");
    gameCamera.AddComponent<ly::scene::CameraComponent>();
    gameCamera.AddSingletonComponent<ly::scene::MainCameraTag>();

    m_CubeEntity = m_Scene->CreateEntity("Cube");
    m_CubeEntity.AddComponent<ly::scene::MeshComponent>(Geometry::GetCube(), m_Shader, m_Texture);
    m_CubeEntity.AddComponent<ly::scene::RenderComponent>();

    m_SceneRuntime->OnViewportResize(glm::uvec2(spec.Width, spec.Height));
    m_SceneRuntime->SetSceneExecState(ly::scene::SceneRuntimeMode::PLAY);
#pragma endregion

#pragma region Inti Scene and Panels
    m_SceneWorkspace                  = ly::MakeScope<ESceneWorkspace>("SceneWorkspace");
    m_EditorContext                   = ly::MakeRef<GlobalEditorContext>();
    m_EditorContext->ActiveScene      = m_Scene;
    m_EditorContext->SceneFramebuffer = m_Framebuffer;
    m_EditorContext->SceneRuntime     = m_SceneRuntime;
    m_SceneWorkspace->OnAttach(m_EditorContext);
#pragma endregion
}

void EditorLayer::OnUpdate(float deltaTime) {
    m_Framebuffer->Bind();
    renderer::RenderCommand::Clear();
    renderer::RenderCommand::SetClearColor(g_ClearColor);

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

    ImGui::ShowDemoWindow();
}
