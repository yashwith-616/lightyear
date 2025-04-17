#include "Lightyear/Editor/ImGuiLayer.h"
#include "GLFW/glfw3.h"
#include "Lightyear/Core/Application.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"

namespace ly {

constexpr float DEFAULT_FRAMETIME = (1.f / 60.f);

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

    ImGui_ImplOpenGL3_Init("#version 410 core");

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

    const float current_time = static_cast<float>(glfwGetTime());
    io.DeltaTime             = (m_Time > 0.f) ? (current_time - m_Time) : DEFAULT_FRAMETIME;
    m_Time                   = current_time;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    static bool show = true;
    ImGui::ShowDemoWindow(&show);

    // Render
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::OnEvent(Event& event) {}

}  // namespace ly
