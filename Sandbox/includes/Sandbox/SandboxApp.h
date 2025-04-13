#pragma once

#include "Lightyear.h"

class SandboxApp : public ly::Application {
public:
	using super = ly::Application;

	SandboxApp();
	~SandboxApp();
	
	virtual void Run() override;
};

ly::Application* CreateApplication() {
	return new SandboxApp();
}