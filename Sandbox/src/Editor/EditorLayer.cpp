#include "Sandbox/Editor/EditorLayer.h"
#include "Sandbox/Core/Camera/EditorCamera.h"
#include "Sandbox/Geometry/Geometry.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace renderer = ly::renderer;

static constexpr ImGuiWindowFlags DockspaceHostFlags = ImGuiWindowFlags_None;
static constexpr float aspect                        = 1.77778;

// Grid Shader
const std::unordered_map<renderer::ShaderType, ly::CPath> g_GridShader = {
    { renderer::ShaderType::Vertex, ASSET_DIR "/Shaders/Vertex/S_Grid.vert" },
    { renderer::ShaderType::Fragment, ASSET_DIR "/Shaders/Fragment/S_Grid.frag" }
};

EditorLayer::EditorLayer() : ly::Layer("Editor") {}

void EditorLayer::OnUpdate(float deltaTime) {
    PollInput(deltaTime);

    m_Framebuffer->Bind();
    renderer::RenderCommand::SetClearColor(glm::vec4(0.f, 0.f, 0.f, 0.5f));
    renderer::RenderCommand::Clear();

    if (m_Scene->IsPaused()) {
        m_Scene->OnUpdateEditor(
            deltaTime, std::static_pointer_cast<ly::renderer::SceneCamera>(m_EditorCamera));
    }

    if (m_Scene->IsRunning()) {
        m_Scene->OnUpdateRuntime(deltaTime);
    }

    m_Framebuffer->Unbind();
}

void EditorLayer::OnAttach() {
    ly::renderer::FramebufferSpecification spec;
    spec.Height   = 1280;
    spec.Width    = 720;
    m_Framebuffer = ly::renderer::Framebuffer::Create(spec);

    m_Texture = ly::renderer::Texture2D::Create(ASSET_DIR "/Textures/T_Grid.png");
    m_Shader  = ly::renderer::Shader::Create("ShaderBg", g_GridShader);

    m_EditorCamera = ly::MakeRef<EditorCamera>(aspect);
    m_EditorCamera->SetSpeed(1.f);

#pragma region Game Scene
    m_Scene                        = ly::MakeRef<ly::scene::Scene>();
    auto new_cam                   = ly::MakeRef<EditorCamera>(aspect);
    ly::scene::Entity cameraEntity = m_Scene->CreateEntity("GameCamera");
    cameraEntity.AddComponent<ly::scene::CameraComponent>(new_cam);
    m_Scene->OnViewportResize(1280, 720);

    /*ly::scene::Entity planeEntity = m_Scene->CreateEntity("Plane");
    planeEntity.AddComponent<ly::scene::MeshComponent>(Geometry::GetPlane(), m_Shader, m_Texture);
    planeEntity.AddComponent<ly::scene::RenderComponent>();*/

    ly::scene::Entity cubeEntity = m_Scene->CreateEntity("Cube");
    cubeEntity.AddComponent<ly::scene::MeshComponent>(Geometry::GetCube(), m_Shader, m_Texture);
    cubeEntity.AddComponent<ly::scene::RenderComponent>();
#pragma endregion

    InitEditorLayouts();
}

void EditorLayer::OnEvent(ly::Event& event) {}

void EditorLayer::OnEditorRender() {
    DrawDockspace();
    m_ViewportPanel.OnImGuiRender();
    ImGui::ShowDemoWindow();
    DrawLogPanel();
    DrawTestPanel();
}

void EditorLayer::InitEditorLayouts() {
    m_ViewportPanel.SetActiveCamera(m_EditorCamera);
    m_ViewportPanel.SetActiveScene(m_Scene);
    m_ViewportPanel.SetFramebuffer(m_Framebuffer);
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
    ImGui::Begin("Test panel");

    static char buffer[256] = "";
    ImGui::InputText("Input", buffer, sizeof(buffer));

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

    if (float val = ly::Input::GetMouseY()) {
        m_EditorCamera->AddPitch(val);
    }

    if (float val = ly::Input::GetMouseX()) {
        m_EditorCamera->AddYaw(val);
    }
}