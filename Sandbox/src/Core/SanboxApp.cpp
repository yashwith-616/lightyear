#include "Sandbox/Core/SandboxApp.h"
#include "Sandbox/Editor/EditorLayer.h"

void SandboxApp::init()
{
    Application::init();
    pushLayer(ly::makeScope<EditorLayer>());
}

void SandboxApp::run() { super::run(); }
