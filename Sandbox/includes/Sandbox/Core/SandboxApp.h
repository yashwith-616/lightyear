#pragma once

#include "Lightyear.h"
#include "Lightyear/Core/Entrypoint.h"

class SandboxApp : public ly::Application {
public:
    using super = ly::Application;

    SandboxApp();
    ~SandboxApp();

    virtual void Run() override;
};

ly::Scope<ly::Application> CreateApplication() {
    return ly::MakeScope<SandboxApp>();
}
