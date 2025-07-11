#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly {

class Event;

struct WindowProps {
    std::string_view Title;
    uint32_t Width;
    uint32_t Height;

    WindowProps(std::string_view title = kDefaultWindowTitle,
                uint32_t width         = kDefaultWindowWidth,
                uint32_t height        = kDefaultWindowHeight)
        : Title(title), Width(width), Height(height) {}
};

/**
 * @brief Interface for a desktop system based Window
 */
class LIGHTYEAR_API Window {
public:
    virtual ~Window() {}

    virtual void OnUpdate()            = 0;
    virtual uint32_t GetWidth() const  = 0;
    virtual uint32_t GetHeight() const = 0;
    virtual float GetTime() const      = 0;

    // Window Attributes
    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
    virtual void SetVSync(bool isEnabled)                          = 0;
    virtual bool IsVSync() const                                   = 0;
    virtual void* GetNativeWindow() const                          = 0;

    static Scope<Window> Create(const WindowProps& props = WindowProps());
};

}  // namespace ly
