#include "Lightyear/Renderer/ImGui/ImGuiLayer.h"
#include "Sandbox/Core/SandboxApp.h"
#include "Sandbox/Layers/ExampleLayer.h"

SandboxApp::SandboxApp() {
    PushLayer(new ly::ImGuiLayer());
    PushLayer(new ExampleLayer());
}

SandboxApp::~SandboxApp() {}

void SandboxApp::Run() {
    super::Run();
}