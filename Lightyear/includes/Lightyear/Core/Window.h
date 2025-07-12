#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly {

class Event;

struct WindowProps {
    std::string_view Title;
    glm::vec2<uint32_t> Size;

    WindowProps(std::string_view title = kDefaultWindowTitle,
                glm::vec2 size         = glm::vec2(kDefaultWindowWidth, kDefaultWindowHeight))
        : Title(title), Size(size) {}
};

/**
 * @brief Interface for a desktop system based Window
 */
class LIGHTYEAR_API Window {
public:
    Window()          = default;
    virtual ~Window() = default;

    /**
     * Create a new window
     *
     * @param props The window properties
     * @return The new Window
     */
    static Scope<Window> Create(const WindowProps& props = WindowProps());

    virtual void OnUpdate()                         = 0;
    [[nodiscard]] virtual glm::vec2<uint32_t> GetSize() const = 0;
    [[nodiscard]] virtual float GetTime() const     = 0;

    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
    virtual void SetVSync(bool isEnabled)                          = 0;
    [[nodiscard]] virtual bool IsVSync() const                     = 0;
    [[nodiscard]] virtual void* GetNativeWindow() const            = 0;
};

}  // namespace ly
