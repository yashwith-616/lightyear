#pragma once

#include "Application.h"
#include "Lightyear/LightYearCore.h"

#ifdef LY_PLATFORM_WINDOWS

extern ly::Scope<ly::Application> CreateApplication();

int main(int argc, char** argv) {
    ly::Log::Init();

    ly::Scope<ly::Application> app = CreateApplication();
    ly::Application::SetApplication(std::move(app));
    ly::Application::Get().Run();
}

#endif