#include "Lightyear/Platform/OpenGL/OpenGLContext.h"
#include <GLFW/glfw3.h>
#include <glad.h>

namespace ly {

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {
    LY_CORE_ASSERT(windowHandle, "Window Handle is null!");
}

void OpenGLContext::Init() {
    glfwMakeContextCurrent(m_WindowHandle);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    LY_CORE_ASSERT(status, "Failed to initialize GLAD");

    // Log System Metrics
    const std::string_view glVendor   = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    const std::string_view glRenderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    const std::string_view glVersion  = reinterpret_cast<const char*>(glGetString(GL_VERSION));

    LY_CORE_LOG(LogType::Info,
                "OpenGL Renderer:\n"
                "\tVendor   : {}\n"
                "\tRenderer : {}\n"
                "\tVersion  : {}",
                glVendor,
                glRenderer,
                glVersion);
}

void OpenGLContext::SwapBuffers() {
    glfwSwapBuffers(m_WindowHandle);
}

}  // namespace ly