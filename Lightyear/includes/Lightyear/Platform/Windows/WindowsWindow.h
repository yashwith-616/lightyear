#pragma once

#include "Lightyear/Core/Window.h"
#include "Lightyear/LightyearCore.h"

struct GLFWwindow;

namespace ly {

namespace renderer {
class RendererContext;
}  // namespace renderer

class LIGHTYEAR_API WindowsWindow : public Window {
public:
    explicit WindowsWindow(const WindowProps& props) : m_Data({props.Title, props.Size}) {}
     ~WindowsWindow() override = default;

    void OnUpdate() override;

    inline float GetTime() const override;
    inline void SetVSync(bool isEnabled) override;

    bool IsVSync() const override {return m_Data.VSync;}
    void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
    glm::vec2<uint32_t> GetSize() const override {return m_Data.WindowSize;}
    void* GetNativeWindow() const override { return static_cast<void*>(m_Window); }

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

    bool m_bIsGLFWInitialized{ false };

private:
    GLFWwindow* m_Window{ nullptr };
    Scope<renderer::RendererContext> m_Context{};

    /**
     * @brief Holds data related to a GLFW window.
     *
     * This struct is set as the `glfwSetWindowUserPointer` and stores window state.
     * It is used within GLFW callbacks to access and update window data, and to dispatch events.
     */
    struct WindowsData {
        std::string_view Title{ "Demo" };
        glm::vec2<uint32_t> WindowSize{ 800, 600 };
        bool VSync{ true };

        EventCallbackFn EventCallback;

        WindowsData(std::string_view title, glm::vec2<uint32_t> windowSize) : Title(title), WindowSize(windowSize) {}
    };

    WindowsData m_Data;
};

}  // namespace ly
