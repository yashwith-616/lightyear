#include "Lightyear/Platform/OpenGL/Renderer/Core/OpenGLContext.h"

LY_DISABLE_WARNINGS_PUSH
#include <GLFW/glfw3.h>
#include <glad.h>
LY_DISABLE_WARNINGS_POP

namespace {
// NOLINTNEXTLINE
void APIENTRY GLDebugCallback(GLenum source,
                              GLenum /*type*/,
                              GLuint /*id*/,
                              GLenum /*severity*/,
                              GLsizei /*length*/,
                              const GLchar* message,
                              const void* /*userParam*/) {
    LY_CORE_LOG(ly::LogType::Error, "[OpenGL Debug] {}", message);
}

}  // namespace

namespace ly::renderer {

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {
    LY_CORE_ASSERT(windowHandle, "Window Handle is null!");
}

void OpenGLContext::Init() {
    glfwMakeContextCurrent(m_WindowHandle);
    const int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    LY_CORE_ASSERT(status, "Failed to initialize GLAD");

    // Log System Metrics
    const std::string_view glVendor        = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    const std::string_view glRenderer      = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    const std::string_view glVersion       = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    const std::string_view glShaderVersion = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

    LY_CORE_LOG(LogType::Info,
                "OpenGL Renderer:\n"
                "\tVendor   : {}\n"
                "\tRenderer : {}\n"
                "\tVersion  : {}\n"
                "\tShaderVersion: {}",
                glVendor,
                glRenderer,
                glVersion,
                glShaderVersion);

#ifdef LY_OPENGL_DEBUG
    int flags{};
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (static_cast<bool>(flags & GL_CONTEXT_FLAG_DEBUG_BIT)) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(GLDebugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
#endif

    int profile{};
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);

    std::string_view glProfile = static_cast<bool>(profile & GL_CONTEXT_CORE_PROFILE_BIT) ? "Core" : "Compatibility";
    LY_CORE_LOG(LogType::Info, "OpenGL Context: {} profile is active", glProfile);
}

void OpenGLContext::SwapBuffers() {
    glfwSwapBuffers(m_WindowHandle);
}

}  // namespace ly::renderer