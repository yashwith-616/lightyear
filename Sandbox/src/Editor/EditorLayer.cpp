#include "Sandbox/Editor/EditorLayer.h"
#include "Sandbox/Core/Camera/EditorCamera.h"
#include "Sandbox/Geometry/Geometry.h"
#include "Sandbox/Helpers/GridRender.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace renderer = ly::renderer;

static constexpr ImGuiWindowFlags DockspaceHostFlags = ImGuiWindowFlags_None;
static constexpr float aspect                        = 1.77778;

// Grid Shader
const std::unordered_map<renderer::ShaderType, ly::CPath> g_GridShader = {
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
    m_Shader  = ly::renderer::Shader::Create("ShaderBg", g_GridShader);
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
    auto new_cam                   = ly::MakeRef<EditorCamera>(aspect);
    ly::scene::Entity cameraEntity = m_Scene->CreateEntity("GameCamera");
    cameraEntity.AddComponent<ly::scene::CameraComponent>(new_cam);
    m_Scene->OnViewportResize(1280, 720);

    m_CubeEntity = m_Scene->CreateEntity("Cube");
    m_CubeEntity.AddComponent<ly::scene::MeshComponent>(Geometry::GetCube(), m_Shader, m_Texture);
    m_CubeEntity.AddComponent<ly::scene::RenderComponent>();

    ly::scene::TransformComponent cubeTransform =
        m_CubeEntity.GetComponent<ly::scene::TransformComponent>();
    cubeTransform.GetTransform();
#pragma endregion

#pragma region Inti Scene and Panels
    m_SceneWorkspace               = ly::MakeScope<ESceneWorkspace>("SceneWorkspace");
    m_EditorContext                = ly::MakeRef<GlobalEditorContext>();
    m_EditorContext->m_ActiveScene = m_Scene;
    m_SceneWorkspace->OnAttach(m_EditorContext);
#pragma endregion

    m_GridRenderer = ly::MakeScope<GridRender>();
}

void EditorLayer::OnEvent(ly::Event& event) {}

void EditorLayer::OnUpdate(float deltaTime) {
    PollInput(deltaTime);

    m_Framebuffer->Bind();
    renderer::RenderCommand::SetClearColor(glm::vec4(0.f, 0.f, 0.f, 0.5f));
    renderer::RenderCommand::Clear();
    m_GridRenderer->Render();

    if (m_Scene->IsPaused()) {
        m_Scene->OnUpdateEditor(
            deltaTime, std::static_pointer_cast<ly::renderer::SceneCamera>(m_EditorCamera));
    }

    if (m_Scene->IsRunning()) {
        m_Scene->OnUpdateRuntime(deltaTime);
    }

    m_Framebuffer->Unbind();
}

void EditorLayer::OnEditorRender() {
    m_SceneWorkspace->OnEditorUpdate();
    DrawDockspace();

    ImGui::ShowDemoWindow();
    m_SceneWorkspace->OnImGuiRender();
}

void EditorLayer::DrawDockspace() {
    ImGuiViewport* vp = ImGui::GetMainViewport();
    if (!vp) {
        LY_LOG(ly::LogType::Error, "Main Viewport is null!");
        return;
    }

    ImGui::SetNextWindowPos(vp->WorkPos);
    ImGui::SetNextWindowSize(vp->WorkSize);
    ImGui::SetNextWindowViewport(vp->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("LyDockspaceHost", nullptr, DockspaceHostFlags);
    ImGui::PopStyleVar(2);

    ImGuiID dockspaceID = ImGui::GetID("LYDockspace");
    ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::End();
}

void EditorLayer::DrawLogPanel() {
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Output");

    if (ImGui::Button("Clear")) {
        // clear the sink
    }

    ImGui::SameLine();
    static bool autoscroll = true;
    ImGui::Checkbox("Autoscroll", &autoscroll);

    ImGui::Separator();
    ImGui::BeginChild("LogScroll", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

    /*for (const auto& line : sink->GetLines()) {
        ImGui::TextUnformatted(line.c_str());
    }*/

    if (autoscroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.f);
    }

    ImGui::EndChild();
    ImGui::End();
}

void EditorLayer::DrawTestPanel() {
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

    ImGui::Begin("Test Panel");
    ly::scene::TransformComponent& cubeTransform =
        m_CubeEntity.GetComponent<ly::scene::TransformComponent>();
    glm::vec3& position = cubeTransform.Translation;
    glm::vec3& rotation = cubeTransform.Rotation;
    if (ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f)) {
        // Optional: log or respond to changes
        LY_CORE_LOG(ly::LogType::Info,
                    "Cube position updated: ({}, {}, {})",
                    position.x,
                    position.y,
                    position.z);
    }

    if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.1f)) {
        // Optional: log or respond to changes
        LY_CORE_LOG(ly::LogType::Info,
                    "Cube rotation updated: ({}, {}, {})",
                    rotation.x,
                    rotation.y,
                    rotation.z);
    }

    ImGui::End();
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
        if (float val = ly::Input::GetMouseY()) {
            float diff = val - m_PrevMouseY;
            m_EditorCamera->AddPitch(diff * deltaTime * m_MouseSensitivity);
            m_PrevMouseY = val;
        }

        if (float val = ly::Input::GetMouseX()) {
            float diff = val - m_PrevMouseX;
            m_EditorCamera->AddYaw(diff * deltaTime * m_MouseSensitivity);
            m_PrevMouseX = val;
        }
    }
}
