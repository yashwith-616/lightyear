#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly {

class Event;

struct WindowProps {
    std::string Title{ kDefaultWindowTitle };
    glm::uvec2 Size{ kDefaultWindowSize };

    WindowProps() = default;
    WindowProps(std::string title, glm::uvec2 size) : Title(std::move(title)), Size(size) {}
};

/**
 * @brief Interface for a desktop system based Window
 */
class LIGHTYEAR_API Window {
public:
    Window()          = default;
    virtual ~Window() = default;

    Window(const Window&)            = delete;
    Window(Window&&)                 = default;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&)      = default;

    /**
     * Create a new window
     *
     * @param props The window properties
     * @return The new Window
     */
    static Scope<Window> Create(const WindowProps& props = WindowProps());

    virtual void Init()     = 0;
    virtual void ShutDown() = 0;
    virtual void OnUpdate() = 0;

    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
    virtual void SetVSync(bool isEnabled)                          = 0;

    [[nodiscard]] virtual glm::uvec2 GetSize() const    = 0;
    [[nodiscard]] virtual float GetTime() const         = 0;
    [[nodiscard]] virtual bool IsVSync() const          = 0;
    [[nodiscard]] virtual void* GetNativeWindow() const = 0;
};

}  // namespace ly
