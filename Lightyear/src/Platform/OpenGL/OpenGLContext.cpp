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
    LY_CORE_ASSERT(status, "Failed to initalize GLAD");
    LY_CORE_LOG(LogType::Info,
                "OpenGL Renderer:\n\tVendor: {0}\n\tRenderer: {1}\n\tVersion: {2}",
                reinterpret_cast<const char*>(glGetString(GL_VENDOR)),
                reinterpret_cast<const char*>(glGetString(GL_RENDERER)),
                reinterpret_cast<const char*>(glGetString(GL_VERSION)));
}

void OpenGLContext::SwapBuffers() {
    glfwSwapBuffers(m_WindowHandle);
}

}  // namespace ly