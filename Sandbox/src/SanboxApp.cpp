#include "Lightyear/Editor/ImGuiLayer.h"
#include "Sandbox/Core/ExampleLayer.h"
#include "Sandbox/SandboxApp.h"

SandboxApp::SandboxApp() {
    PushLayer(new ExampleLayer());
    PushLayer(new ly::ImGuiLayer());
}

SandboxApp::~SandboxApp() {}

void SandboxApp::Run() {
    super::Run();
}