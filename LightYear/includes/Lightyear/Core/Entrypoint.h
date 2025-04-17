#pragma once

#ifdef LY_PLATFORM_WINDOWS

extern ly::Application* CreateApplication();

int main(int argc, char** argv) {
    auto app = CreateApplication();
    app->Run();
    delete app;
}

#endif