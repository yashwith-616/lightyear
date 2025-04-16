#include "Sandbox/SandboxApp.h"
#include "Sandbox/Core/ExampleLayer.h"

SandboxApp::SandboxApp()
{
	PushLayer(new ExampleLayer());
}

SandboxApp::~SandboxApp()
{

}

void SandboxApp::Run()
{
	super::Run();
}