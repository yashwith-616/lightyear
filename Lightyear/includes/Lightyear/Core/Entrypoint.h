#pragma once

#include "Lightyear/Core/Application.h"
#include "Lightyear/LightyearCore.h"

#ifdef LY_PLATFORM_WINDOWS

extern ly::scope<ly::Application> createApplication();

int main(int argc, char** argv) {
    ly::Log::init();
    {
        ly::Application::create(createApplication());
        ly::Application::get().run();
    }
    ly::Log::shutdown();
}
#endif
