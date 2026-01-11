#include "Lightyear/Platform/OpenGL/Renderer/Core/OpenGLContext.h"

LY_DISABLE_WARNINGS_PUSH
#include <GLFW/glfw3.h>
#include <glad/glad.h>
LY_DISABLE_WARNINGS_POP

namespace
{
/**
 * Get OpenGL Issue source origin
 *
 * @param source the source
 * @return
 */
constexpr std::string_view GetOpenGLSourceString(GLenum source)
{
    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        return "API";
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        return "Window System";
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        return "Shader Compiler";
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        return "Third Party";
    case GL_DEBUG_SOURCE_APPLICATION:
        return "Application";
    case GL_DEBUG_SOURCE_OTHER:
        return "Other";
    default:
        return "Unknown Source";
    }
}

/**
 * Get OpenGL issue type
 *
 * @param type the IssueType
 * @return the openGL mapped Issue
 */
constexpr std::string_view GetOpenGLTypeString(GLenum type)
{
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        return "Error";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        return "Deprecated Behavior";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        return "Undefined Behavior";
    case GL_DEBUG_TYPE_PORTABILITY:
        return "Portability";
    case GL_DEBUG_TYPE_PERFORMANCE:
        return "Performance";
    case GL_DEBUG_TYPE_MARKER:
        return "Marker";
    case GL_DEBUG_TYPE_PUSH_GROUP:
        return "Push Group";
    case GL_DEBUG_TYPE_POP_GROUP:
        return "Pop Group";
    case GL_DEBUG_TYPE_OTHER:
        return "Other";
    default:
        return "Unknown Type";
    }
}

/**
 * Map OpenGL severity level to Lightyear severity level
 *
 * @param severity OpenGL severity level
 * @return The lightyear log level
 */
ly::LogType GetLogLevelFromOpenGLSeverity(GLenum severity)
{
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        return ly::LogType::FATAL;
    case GL_DEBUG_SEVERITY_MEDIUM:
        return ly::LogType::Error;
    case GL_DEBUG_SEVERITY_LOW:
        return ly::LogType::WARN;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        return ly::LogType::TRACE;
    default:
        return ly::LogType::INFO;
    }
}

constexpr std::string_view LogTypeToString(ly::LogType type)
{
    switch (type)
    {
    case ly::LogType::TRACE:
        return "Trace";
    case ly::LogType::INFO:
        return "Info";
    case ly::LogType::WARN:
        return "Warn";
    case ly::LogType::Error:
        return "Error";
    case ly::LogType::FATAL:
        return "Fatal";
    default:
        return "Unknown";
    }
}

/**
 * The OpenGL debug callback
 * @param source the callback source/origin
 * @param type the type of issue
 * @param id the debug issue ID
 * @param severity the severity of the issue
 * @param message the error message
 */
void APIENTRY GLDebugCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei /*length*/,
    const GLchar* message,
    const void* /*userParam*/)
{
    const ly::LogType logLevel = GetLogLevelFromOpenGLSeverity(severity);
    LY_CORE_LOG(
        logLevel,
        "[OpenGL Debug - {0} - {1} - ID {2} - Severity {3}]: {4}",
        GetOpenGLSourceString(source),
        GetOpenGLTypeString(type),
        id,
        LogTypeToString(logLevel),
        message);
}
} // namespace

namespace ly::renderer
{

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
{
    LY_CORE_ASSERT(windowHandle, "Window Handle is null!");
}

void OpenGLContext::Init()
{
    glfwMakeContextCurrent(m_WindowHandle);
    const int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    LY_CORE_ASSERT(status, "Failed to initialize GLAD");

    // Log System Metrics
    std::string_view glVendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    std::string_view glRenderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    std::string_view glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    std::string_view glShaderVersion = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

    LY_CORE_LOG(
        LogType::INFO,
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
    if ((flags & GL_CONTEXT_FLAG_DEBUG_BIT) != 0)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(GLDebugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
#endif

    int profile{};
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);

    const bool isCoreProfileSet = (profile & GL_CONTEXT_CORE_PROFILE_BIT) != 0;
    std::string_view glProfile = isCoreProfileSet ? "Core" : "Compatibility";
    LY_CORE_LOG(LogType::INFO, "OpenGL Context: {} profile is active", glProfile);
}

void OpenGLContext::SwapBuffers() { glfwSwapBuffers(m_WindowHandle); }

} // namespace ly::renderer
