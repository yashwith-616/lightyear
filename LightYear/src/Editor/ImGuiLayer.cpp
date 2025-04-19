#include "Lightyear/Editor/ImGuiLayer.h"
#include "GLFW/glfw3.h"
#include "Lightyear/Core/Application.h"
#include "Lightyear/Core/Log.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include "lypch.h"

#define DELEGATE_SINGLE_PARAM(x) std::bind(x, this, std::placeholders::_1)

namespace ly {

constexpr float g_FixedFrameTime         = (1.f / 60.f);
constexpr std::string_view g_GLSLVersion = "#version 410 core";

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

    dispatcher.Dispatch<MouseButtonPressedEvent>(
        DELEGATE_SINGLE_PARAM(&ImGuiLayer::OnMouseButtonPressedEvent));

    dispatcher.Dispatch<MouseButtonReleasedEvent>(
        DELEGATE_SINGLE_PARAM(&ImGuiLayer::OnMouseButtonReleaseEvent));

    dispatcher.Dispatch<MouseScrolledEvent>(
        DELEGATE_SINGLE_PARAM(&ImGuiLayer::OnMouseScrolledEvent));

    dispatcher.Dispatch<MouseMovedEvent>(DELEGATE_SINGLE_PARAM(&ImGuiLayer::OnMouseMovedEvent));

    dispatcher.Dispatch<KeyPressedEvent>(DELEGATE_SINGLE_PARAM(&ImGuiLayer::OnKeyPressedEvent));
    dispatcher.Dispatch<KeyReleasedEvent>(DELEGATE_SINGLE_PARAM(&ImGuiLayer::OnKeyReleasedEvent));
    dispatcher.Dispatch<KeyTypedEvent>(DELEGATE_SINGLE_PARAM(&ImGuiLayer::OnKeyTypedEvent));

    dispatcher.Dispatch<WindowResizeEvent>(DELEGATE_SINGLE_PARAM(&ImGuiLayer::OnWindowResizeEvent));
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
    // io.AddKeyEvent(event.GetKeyCode(), true);
    return false;
}

bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& event) {
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
