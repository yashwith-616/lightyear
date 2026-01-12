#pragma once

#include "Lightyear/Core/Application.h"
#include "Lightyear/LightyearCore.h"

#ifdef LY_PLATFORM_WINDOWS

extern ly::Scope<ly::Application> CreateApplication();

int main(int argc, char** argv) {
    ly::Log::Init();
    {
        ly::Application::Create(CreateApplication());
        ly::Application::Get().Run();
    }
    ly::Log::Shutdown();
}
#endif
