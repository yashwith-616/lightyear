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
    m_Camera               = ly::MakeRef<EditorCamera>(-aspect, aspect, -1.f, 1.f);
}

void EditorLayer::OnUpdate(float deltaTime) {
    m_Framebuffer->Bind();
    renderer::RenderCommand::SetClearColor(glm::vec4(0.f, 0.f, 0.f, 0.5f));
    renderer::RenderCommand::Clear();

    glm::mat4 transform = glm::mat4(1.0f);
    transform           = glm::scale(transform, glm::vec3(2.0f));

    renderer::Renderer::BeginScene(m_Camera);
    renderer::Renderer::Submit(m_Shader, Geometry::GetPlane(), m_Texture, transform);
    renderer::Renderer::EndScene();

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
    DrawDemoSettings();
    ImGui::ShowDemoWindow();
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
    ImGui::Begin("Viewport");
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();

    // Resize framebuffer if necessary
    if ((uint32_t)viewportSize.x != m_Framebuffer->GetSpecification().Width ||
        (uint32_t)viewportSize.y != m_Framebuffer->GetSpecification().Height) {
        m_Framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
        m_Camera->Resize(viewportSize.x, viewportSize.y);
        LY_LOG(
            ly::LogType::Info, "Viewport size Updated: {0}, {1}", viewportSize.x, viewportSize.y);
    }

    ImGui::Image(
        m_Framebuffer->GetColorAttachmentRenderID(), viewportSize, ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}

void EditorLayer::DrawDemoSettings() {
    static bool showDemoWindow = false;
    static bool vsync          = true;
    static ImVec4 clearColor   = ImVec4(1.f, 1.f, 1.f, 1.0f);

    ImGui::Begin("Demo Settings");

    if (ImGui::Checkbox("VSync", &vsync)) {
        ly::Application::Get().GetWindow().SetVSync(vsync);
    }

    if (ImGui::ColorEdit3("Clear Color", (float*)&clearColor)) {
        renderer::RenderCommand::SetClearColor(
            { clearColor.x, clearColor.y, clearColor.z, clearColor.w });
    }

    ImGui::Checkbox("Show ImGui Demo Window", &showDemoWindow);
    ImGui::End();

    if (showDemoWindow) {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }
}
