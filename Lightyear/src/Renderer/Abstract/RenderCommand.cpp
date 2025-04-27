#include "Lightyear/Renderer/Abstract/RenderCommand.h"
#include "Lightyear/Platform/OpenGL/OpenGLRendererAPI.h"
#include "Lightyear/Renderer/Abstract/RendererAPI.h"
#include "Lightyear/Renderer/Primitives/VertexArray.h"

namespace ly::renderer {

RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

inline void RenderCommand::SetClearColor(const glm::vec4& color) {
    s_RendererAPI->SetClearColor(color);
}

inline void RenderCommand::Clear() {
    s_RendererAPI->Clear();
}

void RenderCommand::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
    s_RendererAPI->DrawIndexed(vertexArray);
}

}  // namespace ly::renderer
