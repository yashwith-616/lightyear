#include "Lightyear/Platform/OpenGL/Renderer/Core/OpenGLContext.h"

LY_DISABLE_WARNINGS_PUSH
#include <GLFW/glfw3.h>
#include <glad/glad.h>
LY_DISABLE_WARNINGS_POP

namespace {
/**
 * Get OpenGL Issue source origin
 *
 * @param source the source
 * @return
 */
constexpr std::string_view getOpenGlSourceString(GLenum source) {
    switch (source) {
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
constexpr std::string_view getOpenGlTypeString(GLenum type) {
    switch (type) {
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
ly::LogType getLogLevelFromOpenGlSeverity(GLenum severity) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            return ly::LogType::Fatal;
        case GL_DEBUG_SEVERITY_MEDIUM:
            return ly::LogType::Error;
        case GL_DEBUG_SEVERITY_LOW:
            return ly::LogType::Warn;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            return ly::LogType::Trace;
        default:
            return ly::LogType::Info;
    }
}

constexpr std::string_view logTypeToString(ly::LogType type) {
    switch (type) {
        case ly::LogType::Trace:
            return "Trace";
        case ly::LogType::Info:
            return "Info";
        case ly::LogType::Warn:
            return "Warn";
        case ly::LogType::Error:
            return "Error";
        case ly::LogType::Fatal:
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
void APIENTRY glDebugCallback(GLenum source,
                              GLenum type,
                              GLuint id,
                              GLenum severity,
                              GLsizei /*length*/,
                              GLchar const* message,
                              void const* /*userParam*/) {
    ly::LogType const logLevel = getLogLevelFromOpenGlSeverity(severity);
    LY_CORE_LOG(logLevel,
                "[OpenGL Debug - {0} - {1} - ID {2} - Severity {3}]: {4}",
                getOpenGlSourceString(source),
                getOpenGlTypeString(type),
                id,
                logTypeToString(logLevel),
                message);
}
}  // namespace

namespace ly::renderer {

OpenGlContext::OpenGlContext(GLFWwindow* windowHandle) : m_windowHandle(windowHandle) {
    LY_CORE_ASSERT(windowHandle, "Window Handle is null!");
}

void OpenGlContext::init() {
    glfwMakeContextCurrent(m_windowHandle);
    int const status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    LY_CORE_ASSERT(status, "Failed to initialize GLAD");

    // Log System Metrics
    std::string_view glVendor        = reinterpret_cast<char const*>(glGetString(GL_VENDOR));
    std::string_view glRenderer      = reinterpret_cast<char const*>(glGetString(GL_RENDERER));
    std::string_view glVersion       = reinterpret_cast<char const*>(glGetString(GL_VERSION));
    std::string_view glShaderVersion = reinterpret_cast<char const*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

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
    if ((flags & GL_CONTEXT_FLAG_DEBUG_BIT) != 0) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
#endif

    int profile{};
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);

    bool const isCoreProfileSet = (profile & GL_CONTEXT_CORE_PROFILE_BIT) != 0;
    std::string_view glProfile  = isCoreProfileSet ? "Core" : "Compatibility";
    LY_CORE_LOG(LogType::Info, "OpenGL Context: {} profile is active", glProfile);
}

void OpenGlContext::swapBuffers() {
    glfwSwapBuffers(m_windowHandle);
}

}  // namespace ly::renderer