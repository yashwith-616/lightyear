#include "Lightyear/Editor/ImGUILayer.h"
#include "Lightyear/Core/Application.h"
#include "Lightyear/Events/ApplicationEvent.h"
#include "Lightyear/Events/EditorEvent.h"
#include "Lightyear/Events/KeyEvent.h"
#include "Lightyear/Events/MouseEvent.h"
#include "Lightyear/Platform/Windows/WindowsWindow.h"

#include <lypch.h>

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <imgui_internal.h>

namespace ly {

ImGUILayer::ImGUILayer() : Layer("ImGUILayer") {}

ImGUILayer::~ImGUILayer() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGUILayer::OnAttach() {
    LY_CORE_LOG(LogType::Info, "Initialized ImGUI");

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding              = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION.data());
}

void ImGUILayer::OnDetach() {
    LY_CORE_LOG(LogType::Trace, "Destroying IMGUI Editor Layer!");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGUILayer::OnEvent(Event& event) {
    EventDispatcher dispatcher{ event };

    // Imgui Mouse Events
    dispatcher.Dispatch<MouseButtonPressedEvent>(
        [this](MouseButtonPressedEvent& e) { return OnMouseButtonPressedEvent(e); });
    dispatcher.Dispatch<MouseButtonReleasedEvent>(
        [this](MouseButtonReleasedEvent& e) { return OnMouseButtonReleaseEvent(e); });
    dispatcher.Dispatch<MouseScrolledEvent>(
        [this](MouseScrolledEvent& e) { return OnMouseScrolledEvent(e); });
    dispatcher.Dispatch<MouseMovedEvent>(
        [this](MouseMovedEvent& e) { return OnMouseMovedEvent(e); });

    // Imgui Key Events
    dispatcher.Dispatch<KeyPressedEvent>(
        [this](KeyPressedEvent& e) { return OnKeyPressedEvent(e); });
    dispatcher.Dispatch<KeyReleasedEvent>(
        [this](KeyReleasedEvent& e) { return OnKeyReleasedEvent(e); });
    dispatcher.Dispatch<KeyTypedEvent>([this](KeyTypedEvent& e) { return OnKeyTypedEvent(e); });

    // Imgui Window Events
    dispatcher.Dispatch<WindowResizeEvent>(
        [this](WindowResizeEvent& e) { return OnWindowResizeEvent(e); });
}

void ImGUILayer::OnEditorRender() {}

/**
 * @brief Initialize ImGUI backends such as Window manager and the RHI
 */
void ImGUILayer::Begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

/**
 * @brief Prepare ImGUI for next update. Render the current update.
 */
void ImGUILayer::End() {
    ImGuiIO& io      = ImGui::GetIO();
    Application& app = Application::Get();
    io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

#pragma region ImGui Callbacks

bool ImGUILayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& event) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseButtonEvent(event.GetMouseButton(), true);
    return false;
}

bool ImGUILayer::OnMouseButtonReleaseEvent(MouseButtonReleasedEvent& event) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseButtonEvent(event.GetMouseButton(), false);
    return false;
}

bool ImGUILayer::OnMouseMovedEvent(MouseMovedEvent& event) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent(event.GetX(), event.GetY());
    return false;
}

bool ImGUILayer::OnMouseScrolledEvent(MouseScrolledEvent& event) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseWheelEvent(event.GetXOffset(), event.GetYOffset());
    return false;
}

bool ImGUILayer::OnKeyPressedEvent(KeyPressedEvent& event) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddKeyEvent(static_cast<ImGuiKey>(event.GetKeyCode()), true);
    return false;
}

bool ImGUILayer::OnKeyReleasedEvent(KeyReleasedEvent& event) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddKeyEvent(static_cast<ImGuiKey>(event.GetKeyCode()), false);
    return false;
}

bool ImGUILayer::OnKeyTypedEvent(KeyTypedEvent& event) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacter(event.GetKeyCode());
    return false;
}

bool ImGUILayer::OnWindowResizeEvent(WindowResizeEvent& event) {
    ImGuiIO& io    = ImGui::GetIO();
    io.DisplaySize = ImVec2(event.GetWidth(), event.GetHeight());
    return false;
}

#pragma endregion

}  // namespace ly
