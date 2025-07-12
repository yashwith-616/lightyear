#include "Lightyear/Platform/OpenGL/Renderer/Core/OpenGLRendererAPI.h"
#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLVertexArray.h"
#include "Lightyear/Renderer/Primitives/Buffer.h"

LY_DISABLE_WARNINGS_PUSH
#include <glad.h>
LY_DISABLE_WARNINGS_POP

#pragma region OpenGL Debugging
// NOLINTBEGIN
namespace {
void OpenGLMessageCallback(unsigned source,
                           unsigned type,
                           unsigned id,
                           unsigned severity,
                           int length,
                           const char* message,
                           const void* userParam) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            LY_CORE_LOG(ly::LogType::Fatal, "{0}", message);
            return;
        case GL_DEBUG_SEVERITY_MEDIUM:
            LY_CORE_LOG(ly::LogType::Error, "{0}", message);
            return;
        case GL_DEBUG_SEVERITY_LOW:
            LY_CORE_LOG(ly::LogType::Warn, "{0}", message);
            return;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            LY_CORE_LOG(ly::LogType::Trace, "{0}", message);
            return;
        default:
            LY_CORE_ASSERT(false, "Unknown severity level!");
            return;
    }

    LY_CORE_ASSERT(false, "Unknown severity level!");
}

/**
 * @brief Initialize openGL debugging callbacks and error control
 */
void InitDebugging() {
#ifdef LY_OPENGL_DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLMessageCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
}

/**
 * @brief Checks for any openGL errors after initializing the openGL state
 */
void CheckOpenGLErrors() {
#ifdef LY_OPENGL_DEBUG
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        LY_CORE_LOG(ly::LogType::Error, "OpenGL error: {0}", error);
    }
#endif
}

}  // namespace
// NOLINTEND
#pragma endregion OpenGL Debug

namespace ly::renderer {

void OpenGLRendererAPI::Init() {
    InitDebugging();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);

    CheckOpenGLErrors();
}

void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    glViewport(x, y, width, height);
}

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
    glClearColor(color.x, color.y, color.z, color.a);
}

/**
 * @brief Clear any test buffers that is initialized by openGL
 * // TODO: May use Stencil Buffer in the future
 */
void OpenGLRendererAPI::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) {
    vertexArray->Bind();
    const uint32_t count = indexCount != 0 ? indexCount : vertexArray->GetIndexBuffer().GetCount();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

void OpenGLRendererAPI::DrawLines(const VertexArray& vertexArray, uint32_t vertexCount) {
    vertexArray.Bind();
    glDrawArrays(GL_LINES, 0, vertexCount);
}

void OpenGLRendererAPI::SetLineWidth(float width) {
    glLineWidth(width);
}

}  // namespace ly::renderer
