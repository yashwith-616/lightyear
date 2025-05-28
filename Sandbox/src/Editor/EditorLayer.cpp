#include "Sandbox/Editor/EditorLayer.h"
#include "Sandbox/Core/Camera/EditorCamera.h"
#include "Sandbox/Geometry/Geometry.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace renderer = ly::renderer;

static constexpr ImGuiWindowFlags DockspaceHostFlags = ImGuiWindowFlags_None;

// Grid Shader
const std::unordered_map<renderer::ShaderType, ly::CPath> g_GridShader = {
    { renderer::ShaderType::Vertex, ASSET_DIR "/Shaders/Vertex/S_Grid.vert" },
    { renderer::ShaderType::Fragment, ASSET_DIR "/Shaders/Fragment/S_Grid.frag" }
};

EditorLayer::EditorLayer() : ly::Layer("Editor") {
    m_Texture = ly::renderer::Texture2D::Create(ASSET_DIR "/Textures/T_Grid.png");
    m_Shader  = ly::renderer::Shader::Create("ShaderBg", g_GridShader);

    constexpr float aspect = 1.77778;

    m_Scene                        = ly::MakeRef<ly::scene::Scene>();
    ly::scene::Entity cameraEntity = m_Scene->CreateEntity("EditorCamera");
    cameraEntity.AddComponent<ly::scene::CameraComponent>(ly::MakeRef<EditorCamera>(aspect));
    m_Scene->OnViewportResize(1280, 720);

    ly::scene::Entity planeEntity = m_Scene->CreateEntity("Plane");
    planeEntity.AddComponent<ly::scene::MeshComponent>(Geometry::GetPlane(), m_Shader, m_Texture);
    planeEntity.AddComponent<ly::scene::RenderComponent>();

    ly::scene::Entity cubeEntity = m_Scene->CreateEntity("Cube");
    cubeEntity.AddComponent<ly::scene::MeshComponent>(Geometry::GetCube(), m_Shader, m_Texture);
    cubeEntity.AddComponent<ly::scene::RenderComponent>();
}

void EditorLayer::OnUpdate(float deltaTime) {
    m_Framebuffer->Bind();
    renderer::RenderCommand::SetClearColor(glm::vec4(0.f, 0.f, 0.f, 0.5f));
    renderer::RenderCommand::Clear();

    m_Scene->OnUpdateRuntime(deltaTime);

    m_Framebuffer->Unbind();
}

void EditorLayer::OnAttach() {
    ly::renderer::FramebufferSpecification spec;
    spec.Height   = 1280;
    spec.Width    = 720;
    m_Framebuffer = ly::renderer::Framebuffer::Create(spec);
}

void EditorLayer::OnEvent(ly::Event& event) {}

void EditorLayer::OnEditorRender() {
    DrawDockspace();
    DrawViewport();
    ImGui::ShowDemoWindow();
    DrawLogPanel();
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

void EditorLayer::DrawViewport() {
    ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_MenuBar);

    // === Top Menu Bar (for play/pause button) ===
    if (ImGui::BeginMenuBar()) {
        if (m_Scene->IsRunning()) {
            if (ImGui::Button("⏸ Pause")) {
                m_Scene->SetSceneExecState(ly::scene::SS_PAUSED);
                LY_LOG(ly::LogType::Info, "Scene paused");
            }
        } else {
            if (ImGui::Button("▶ Play")) {
                m_Scene->SetSceneExecState(ly::scene::SS_RUNNING);
                LY_LOG(ly::LogType::Info, "Scene running");
            }
        }
        ImGui::EndMenuBar();
    }

    /// === Viewport resizing ===
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    if ((uint32_t)viewportSize.x != m_Framebuffer->GetSpecification().Width ||
        (uint32_t)viewportSize.y != m_Framebuffer->GetSpecification().Height) {
        m_Framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
        m_Scene->OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
        LY_LOG(
            ly::LogType::Info, "Viewport size updated: {0}, {1}", viewportSize.x, viewportSize.y);
    }

    // === Viewport rendering ===
    ImGui::Image(
        m_Framebuffer->GetColorAttachmentRenderID(), viewportSize, ImVec2(0, 1), ImVec2(1, 0));

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
