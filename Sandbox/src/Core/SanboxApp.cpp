#include "Sandbox/Core/SandboxApp.h"
#include "Sandbox/Editor/EditorLayer.h"

void SandboxApp::Init() {
    Application::Init();
    PushLayer(ly::MakeScope<EditorLayer>());
    PushLayer(ly::MakeScope<ly::ImGUILayer>());
}

void SandboxApp::Run() {
    super::Run();
}