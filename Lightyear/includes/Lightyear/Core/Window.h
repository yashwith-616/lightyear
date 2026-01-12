#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly {

class Event;

struct WindowProps {
    std::string title{ k_defaultWindowTitle };
    glm::uvec2 size{ k_defaultWindowSize };

    WindowProps() = default;
    WindowProps(std::string title, glm::uvec2 size) : title(std::move(title)), size(size) {}
};

/**
 * @brief Interface for a desktop system based Window
 */
class LIGHTYEAR_API Window {
public:
    Window()          = default;
    virtual ~Window() = default;

    Window(Window const&)            = delete;
    Window(Window&&)                 = default;
    Window& operator=(Window const&) = delete;
    Window& operator=(Window&&)      = default;

    /**
     * Create a new window
     *
     * @param props The window properties
     * @return The new Window
     */
    static scope<Window> create(WindowProps const& props = WindowProps());

    virtual void init()     = 0;
    virtual void shutDown() = 0;
    virtual void onUpdate() = 0;

    virtual void setEventCallback(eventCallbackFn const& callback) = 0;
    virtual void setVSync(bool isEnabled)                          = 0;

    [[nodiscard]] virtual glm::uvec2 getSize() const    = 0;
    [[nodiscard]] virtual float getTime() const         = 0;
    [[nodiscard]] virtual bool isVSync() const          = 0;
    [[nodiscard]] virtual void* getNativeWindow() const = 0;
};

}  // namespace ly
