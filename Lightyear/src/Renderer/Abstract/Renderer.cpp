#include "Lightyear/Renderer/Abstract/Renderer.h"
#include "Lightyear/Renderer/Abstract/RenderCommand.h"

namespace ly::renderer {

void Renderer::Init() {}

void Renderer::Shutdown() {}

void Renderer::OnWindowResize(uint32_t width, uint32_t height) {}

void Renderer::BeginScene() {}

void Renderer::EndScene() {}

void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray) {
    RenderCommand::DrawIndexed(vertexArray);
}

}  // namespace ly::renderer