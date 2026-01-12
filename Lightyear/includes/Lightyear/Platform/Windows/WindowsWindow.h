#pragma once

#include "Lightyear/Core/Window.h"
#include "Lightyear/LightyearCore.h"
#include "Lightyear/Renderer/Abstract/RendererContext.h"

struct GLFWwindow;

namespace ly {

class LIGHTYEAR_API WindowsWindow : public Window {
public:
    explicit WindowsWindow(const WindowProps& props) : m_Data({ props.Title, props.Size }) {}
    ~WindowsWindow() override = default;

    void OnUpdate() override;

    void SetVSync(bool enable) override;
    void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }

    [[nodiscard]] float GetTime() const override;
    [[nodiscard]] bool IsVSync() const override { return m_Data.IsVSyncEnabled; }
    [[nodiscard]] glm::uvec2 GetSize() const override { return m_Data.WindowSize; }
    [[nodiscard]] void* GetNativeWindow() const override { return m_Window; }

protected:
    bool m_IsGLFWInitialized{ false };

    /**
     * @brief Initializes GLFW window with the given properties such as context, title, width,
     * height, and other related settings.
     */
    void Init() override;

    /**
     * @brief Closes the GLFW window context and terminates GLFW. Called in the destructor.
     *
     * Override this function to perform any additional cleanup operations.
     */

    void ShutDown() override;

    /**
     * @brief Sets up all GLFW window callbacks (e.g., window resize, close, key/button presses).
     *
     * This class handles the registration and binding of all necessary GLFW callbacks
     * related to window and input events.
     */
    virtual void SetupWindowCallbacks();

private:
    /**
     * @brief Holds data related to a GLFW window.
     *
     * This struct is set as the `glfwSetWindowUserPointer` and stores window state.
     * It is used within GLFW callbacks to access and update window data, and to dispatch events.
     */
    struct WindowsData {
        std::string Title{ kNOTSET };
        glm::uvec2 WindowSize{ kDefaultWindowSize };
        bool IsVSyncEnabled{ true };

        EventCallbackFn EventCallback;

        WindowsData(std::string title, glm::uvec2 windowSize) : Title(std::move(title)), WindowSize(windowSize) {}
    };

    GLFWwindow* m_Window{ nullptr };
    Scope<renderer::RendererContext> m_Context{};
    WindowsData m_Data;

    static void SetupGLFWWindowHints();
};

}  // namespace ly
