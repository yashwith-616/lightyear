#include "Lightyear/Platform/OpenGL/OpenGLContext.h"
#include <GLFW/glfw3.h>
#include <glad.h>

namespace ly::renderer {

static void APIENTRY GLDebugCallback(GLenum source,
                                     GLenum type,
                                     GLuint id,
                                     GLenum severity,
                                     GLsizei length,
                                     const GLchar* message,
                                     const void* userParam) {
    LY_CORE_LOG(LogType::Error, "[OpenGL Debug] {}", message);
}

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {
    LY_CORE_ASSERT(windowHandle, "Window Handle is null!");
}

OpenGLContext::~OpenGLContext() {
    LY_CORE_LOG(LogType::Info, "Render context is being destroyed!");
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

#ifdef LY_DEBUG
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(GLDebugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
#endif
}

void OpenGLContext::SwapBuffers() {
    glfwSwapBuffers(m_WindowHandle);
}

}  // namespace ly::renderer