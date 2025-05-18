#include "Sandbox/Core/SandboxApp.h"
#include "Sandbox/Layers/ExampleLayer.h"

SandboxApp::SandboxApp() {}

void SandboxApp::Init() {
    Application::Init();
    PushLayer(ly::MakeScope<ExampleLayer>());
}

void SandboxApp::Run() {
    super::Run();
}