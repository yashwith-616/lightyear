#include "Lightyear/Renderer/Abstract/Renderer.h"

namespace ly::renderer {

void Renderer::Init() {}

void Renderer::Shutdown() {}

void Renderer::OnWindowResize(uint32_t width, uint32_t height) {}

void Renderer::BeginScene() {}

void Renderer::EndScene() {}

void ly::renderer::Renderer::Submit() {}

}  // namespace ly::renderer