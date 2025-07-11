#pragma once

#include "Lightyear.h"
#include "Lightyear/Core/Entrypoint.h"

class SandboxApp : public ly::Application {
public:
    using super = ly::Application;

    SandboxApp()          = default;
    virtual ~SandboxApp() = default;

    virtual void Init() override;
    virtual void Run() override;
};

inline ly::Scope<ly::Application> CreateApplication() {
    return ly::MakeScope<SandboxApp>();
}
