#pragma once

#include "Lightyear.h"
#include "Lightyear/Core/Entrypoint.h"

class SandboxApp : public ly::Application {
public:
    using super = Application;

    SandboxApp()           = default;
    ~SandboxApp() override = default;

    void Init() override;
    void Run() override;
};

inline ly::Scope<ly::Application> CreateApplication() {
    return ly::MakeScope<SandboxApp>();
}
