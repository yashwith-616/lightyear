#include "Lightyear/Renderer/ImGui/ImGuiLayer.h"
#include <lypch.h>
#include "Lightyear/Core/Application.h"
#include "Lightyear/Events/ApplicationEvent.h"
#include "Lightyear/Events/KeyEvent.h"
#include "Lightyear/Events/MouseEvent.h"
#include "Lightyear/Platform/Windows/WindowsWindow.h"

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

namespace ly {

constexpr float g_FixedFrameTime         = (1.f / 60.f);
constexpr std::string_view g_GLSLVersion = "#version 410 core";

ImGuiLayer::ImGuiLayer() : Layer("ImGUILayer") {}

ImGuiLayer::~ImGuiLayer() {
    // Destroy ImGUI
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::OnAttach() {
    // Setup Dear ImGui context
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;

    ImGui_ImplOpenGL3_Init(g_GLSLVersion.data());

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple
    // fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the
    // font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those
    // errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture
    // when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below
    // will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher
    // quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to
    // write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the
    // "fonts/" folder. See Makefile.emscripten for details. io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
    // nullptr, io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != nullptr);
}

void ImGuiLayer::OnDetach() {}

void ImGuiLayer::OnUpdate() {
    ImGuiIO& io    = ImGui::GetIO();
    Window& window = Application::Get().GetWindow();
    io.DisplaySize = ImVec2(window.GetWidth(), window.GetHeight());

    const float currentTime = static_cast<float>(glfwGetTime());
    io.DeltaTime            = (m_Time > 0.f) ? (currentTime - m_Time) : g_FixedFrameTime;
    m_Time                  = currentTime;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    static bool show = true;
    ImGui::ShowDemoWindow(&show);

    // Render
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::OnEvent(Event& event) {
    EventDispatcher dispatcher{ event };

    // Imgui Mouse Events
    dispatcher.Dispatch<MouseButtonPressedEvent>(
        [this](MouseButtonPressedEvent& e) { return OnMouseButtonPressedEvent(e); });
    dispatcher.Dispatch<MouseButtonReleasedEvent>(
        [this](MouseButtonReleasedEvent& e) { return OnMouseButtonReleaseEvent(e); });
    dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& e) { return OnMouseScrolledEvent(e); });
    dispatcher.Dispatch<MouseMovedEvent>([this](MouseMovedEvent& e) { return OnMouseMovedEvent(e); });

    // Imgui Key Events
    dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& e) { return OnKeyPressedEvent(e); });
    dispatcher.Dispatch<KeyReleasedEvent>([this](KeyReleasedEvent& e) { return OnKeyReleasedEvent(e); });
    dispatcher.Dispatch<KeyTypedEvent>([this](KeyTypedEvent& e) { return OnKeyTypedEvent(e); });

    // Imgui Window Events
    dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) { return OnWindowResizeEvent(e); });
}

#pragma region ImGui Callbacks

bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& event) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseButtonEvent(event.GetMouseButton(), true);
    return false;
}

bool ImGuiLayer::OnMouseButtonReleaseEvent(MouseButtonReleasedEvent& event) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseButtonEvent(event.GetMouseButton(), false);
    return false;
}

bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& event) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent(event.GetX(), event.GetY());
    return false;
}

bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& event) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseWheelEvent(event.GetXOffset(), event.GetYOffset());
    return false;
}

bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& event) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddKeyEvent(static_cast<ImGuiKey>(event.GetKeyCode()), true);
    return false;
}

bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& event) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddKeyEvent(static_cast<ImGuiKey>(event.GetKeyCode()), false);
    return false;
}

bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& event) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacter(event.GetKeyCode());
    return false;
}

bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& event) {
    ImGuiIO& io    = ImGui::GetIO();
    io.DisplaySize = ImVec2(event.GetWidth(), event.GetHeight());
    return false;
}

#pragma endregion

}  // namespace ly
