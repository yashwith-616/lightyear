#pragma once

#include "Lightyear/Core/Window.h"
#include "Lightyear/LightyearCore.h"

struct GLFWwindow;

namespace ly {

class RendererContext;

class LIGHTYEAR_API WindowsWindow : public Window {
public:
    WindowsWindow(const WindowProps& props);
    virtual ~WindowsWindow();

    virtual void OnUpdate() override;

    inline uint32_t GetWidth() const override { return m_Data.Width; }
    inline uint32_t GetHeight() const override { return m_Data.Height; }

    // Window Attributes
    virtual void SetVSync(bool isEnabled) override;
    virtual bool IsVSync() const override;
    virtual void* GetNativeWindow() const override { return static_cast<void*>(m_Window); }
    inline void SetEventCallback(const EventCallbackFn& callback) override {
        m_Data.EventCallback = callback;
    }

    virtual float GetTime() const override;

protected:
    /**
     * @brief Initializes GLFW window with the given properties such as context, title, width,
     * height, and other related settings.
     *
     * @param props The window properties used to configure the GLFW window.
     */
    virtual void Init(const WindowProps& props);

    /**
     * @brief Closes the GLFW window context and terminates GLFW. Called in the destructor.
     *
     * Override this function to perform any additional cleanup operations.
     */

    virtual void ShutDown();

    /**
     * @brief Sets up all GLFW window callbacks (e.g., window resize, close, key/button presses).
     *
     * This class handles the registration and binding of all necessary GLFW callbacks
     * related to window and input events.
     */
    virtual void SetupWindowCallbacks();

private:
    GLFWwindow* m_Window;
    Scope<RendererContext> m_Context;

    /**
     * @brief Holds data related to a GLFW window.
     *
     * This struct is set as the `glfwSetWindowUserPointer` and stores window state.
     * It is used within GLFW callbacks to access and update window data, and to dispatch events.
     */
    struct WindowsData {
        CLabel Title{ "Demo" };
        uint32_t Height{ 0 };
        uint32_t Width{ 0 };
        bool VSync{ true };

        EventCallbackFn EventCallback;
    };

    WindowsData m_Data{};
};

}  // namespace ly
