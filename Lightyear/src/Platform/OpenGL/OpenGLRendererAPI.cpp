#include "Lightyear/Platform/OpenGL/OpenGLRendererAPI.h"
#include <glad.h>
#include "Lightyear/Renderer/Primitives/Buffer.h"

namespace ly::renderer {

void OpenGLRendererAPI::Init() {
#ifdef LY_OPENGL_DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLMessageCallback, nullptr);
    glDebugMessageControl(
        GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
}

void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    glViewport(x, y, width, height);
}

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
    glClearColor(color.x, color.y, color.z, color.a);
}

void OpenGLRendererAPI::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const VertexArray& vertexArray, uint32_t indexCount) {
    // vertexArray->bind();
    // uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
    // glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr)
}

void OpenGLRendererAPI::DrawLines(const VertexArray& vertexArray, uint32_t vertexCount) {
    // vertexArray->bind();
    // glDrawArrays(GL_LINES, 0, vertexCount);
}

void OpenGLRendererAPI::SetLineWidth(float width) {
    glLineWidth(width);
}

void OpenGLMessageCallback(unsigned source,
                           unsigned type,
                           unsigned id,
                           unsigned severity,
                           int length,
                           const char* message,
                           const void* userParam) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH: LY_CORE_LOG(LogType::Fatal, message); return;
        case GL_DEBUG_SEVERITY_MEDIUM: LY_CORE_LOG(LogType::Error, message); return;
        case GL_DEBUG_SEVERITY_LOW: LY_CORE_LOG(LogType::Warn, message); return;
        case GL_DEBUG_SEVERITY_NOTIFICATION: LY_CORE_LOG(LogType::Trace, message); return;
    }

    LY_CORE_ASSERT(false, "Unknown severity level!");
}

}  // namespace ly::renderer