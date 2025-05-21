#include "Sandbox/Core/SandboxApp.h"
#include "Sandbox/Editor/EditorLayer.h"
#include "Sandbox/Extras/ExampleLayer.h"

SandboxApp::SandboxApp() {}

void SandboxApp::Init() {
    Application::Init();
    PushLayer(ly::MakeScope<EditorLayer>());
}

void SandboxApp::Run() {
    super::Run();
}