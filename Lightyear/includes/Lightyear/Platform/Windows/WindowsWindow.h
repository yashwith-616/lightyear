#pragma once

#include <GLFW/glfw3.h>
#include <Lightyear/LightyearCore.h>

#include "Lightyear/Core/Window.h"
#include "Lightyear/Platform/OpenGL/Renderer/Core/OpenGLContext.h"
#include "Lightyear/Renderer/Abstract/RendererContext.h"

// NOLINTNEXTLINE
struct GLFWwindow;

namespace ly
{

class LIGHTYEAR_API WindowsWindow : public Window
{
public:
    explicit WindowsWindow(WindowProps const& props) : m_data({props.title, props.size}) {}
    ~WindowsWindow() override = default;

    void onUpdate() override;

    void setVSync(bool enable) override;
    void setEventCallback(eventCallbackFn const& callback) override { m_data.eventCallback = callback; }

    [[nodiscard]] float getTime() const override;
    [[nodiscard]] bool isVSync() const override { return m_data.isVSyncEnabled; }
    [[nodiscard]] glm::uvec2 getSize() const override { return m_data.windowSize; }
    [[nodiscard]] void* getNativeWindow() const override { return m_window; }

protected:
    bool m_isGlfwInitialized{false};

    /**
     * @brief Initializes GLFW window with the given properties such as context, title, width,
     * height, and other related settings.
     */
    void init() override;

    /**
     * @brief Closes the GLFW window context and terminates GLFW. Called in the destructor.
     *
     * Override this function to perform any additional cleanup operations.
     */

    void shutDown() override;

    /**
     * @brief Sets up all GLFW window callbacks (e.g., window resize, close, key/button presses).
     *
     * This class handles the registration and binding of all necessary GLFW callbacks
     * related to window and input events.
     */
    virtual void setupWindowCallbacks();

private:
    /**
     * @brief Holds data related to a GLFW window.
     *
     * This struct is set as the `glfwSetWindowUserPointer` and stores window state.
     * It is used within GLFW callbacks to access and update window data, and to dispatch events.
     */
    struct WindowsData
    {
        std::string title{k_kNotset};
        glm::uvec2 windowSize{k_defaultWindowSize};
        bool isVSyncEnabled{true};

        eventCallbackFn eventCallback;

        WindowsData(std::string title, glm::uvec2 windowSize) : title(std::move(title)), windowSize(windowSize) {}
    };

    GLFWwindow* m_window{nullptr};
    scope<renderer::RendererContext> m_context{};
    WindowsData m_data;

    static void setupGlfwWindowHints();
};

} // namespace ly
