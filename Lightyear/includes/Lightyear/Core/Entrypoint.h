#pragma once

#include "Application.h"
#include "Lightyear/LightYearCore.h"

#ifdef LY_PLATFORM_WINDOWS

extern ly::Application* CreateApplication();

int main(int argc, char** argv) {
    ly::Log::Init();

    auto app = CreateApplication();
    app->Run();
    delete app;
}

#endif