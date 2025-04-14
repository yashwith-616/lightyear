#pragma once

#include "Lightyear/LightyearAPI.h"
#include "lypch.h"

namespace ly {

class Window;

class LIGHTYEAR_API Application {
public:
    Application();
    ~Application();

    virtual void Run();

private:
    std::unique_ptr<Window> m_Window;
    bool m_Running = true;
};
}
