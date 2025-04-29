#include "Lightyear/Renderer/ImGui/ImGuiLayer.h"
#include "Sandbox/Core/ExampleLayer.h"
#include "Sandbox/SandboxApp.h"

SandboxApp::SandboxApp() {
    PushLayer(new ly::ImGuiLayer());
    PushLayer(new ExampleLayer());
}

SandboxApp::~SandboxApp() {}

void SandboxApp::Run() {
    super::Run();
}