#include "Lightyear/Platform/OpenGL/OpenGLRendererAPI.h"
#include <glad.h>
#include "Lightyear/Platform/OpenGL/OpenGLVertexArray.h"
#include "Lightyear/Renderer/Primitives/Buffer.h"

namespace ly::renderer {

static void OpenGLMessageCallback(unsigned source,
                                  unsigned type,
                                  unsigned id,
                                  unsigned severity,
                                  int length,
                                  const char* message,
                                  const void* userParam) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            LY_CORE_LOG(LogType::Fatal, "{0}", message);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            LY_CORE_LOG(LogType::Error, "{0}", message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            LY_CORE_LOG(LogType::Warn, "{0}", message);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            LY_CORE_LOG(LogType::Trace, "{0}", message);
            break;
        default:
            LY_CORE_ASSERT(false, "Unknown severity level!");
            break;
    }

    LY_CORE_ASSERT(false, "Unknown severity level!");
}

#pragma region OpenGL Debug
/**
 * @brief Initialize openGL debugging callbacks and error control
 */
static void InitDebugging() {
#ifdef LY_OPENGL_DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLMessageCallback, nullptr);
    glDebugMessageControl(
        GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
}

/**
 * @brief Checks for any openGL errors after initializing the openGL state
 */
static void CheckOpenGLErrors() {
#ifdef LY_OPENGL_DEBUG
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        LY_CORE_LOG(LogType::Error, "OpenGL error: {0}", error);
    }
#endif
}

#pragma endregion OpenGL Debug

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

void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray,
                                    uint32_t indexCount) {
    vertexArray->Bind();
    uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer().GetCount();

    // LY_CORE_LOG(LogType::Trace, "Trigger Render");
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
