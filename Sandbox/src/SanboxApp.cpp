#include "Sandbox/Core/ExampleLayer.h"
#include "Sandbox/SandboxApp.h"

SandboxApp::SandboxApp() {
    PushLayer(new ExampleLayer());
}

SandboxApp::~SandboxApp() {}

void SandboxApp::Run() {
    super::Run();
}