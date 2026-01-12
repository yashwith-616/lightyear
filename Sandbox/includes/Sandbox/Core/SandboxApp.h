#pragma once

#include "Lightyear.h"
#include "Lightyear/Core/Entrypoint.h"

class SandboxApp : public ly::Application {
public:
    using super = Application;

    SandboxApp()           = default;
    ~SandboxApp() override = default;

    void init() override;
    void run() override;
};

inline ly::scope<ly::Application> createApplication() {
    return ly::makeScope<SandboxApp>();
}
