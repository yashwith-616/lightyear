#include "Lightyear/Renderer/ImGui/ImGuiLayer.h"
#include "Sandbox/Core/SandboxApp.h"
#include "Sandbox/Layers/ExampleLayer.h"

SandboxApp::SandboxApp() {
    PushLayer(ly::MakeScope<ExampleLayer>());
}

SandboxApp::~SandboxApp() {}

void SandboxApp::Run() {
    super::Run();
}