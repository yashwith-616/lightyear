#include "Lightyear/Renderer/Abstract/Renderer.h"
#include "Lightyear/Renderer/Abstract/RenderCommand.h"
#include "Lightyear/Renderer/Abstract/RendererAPI.h"
#include "Lightyear/Renderer/Camera/Camera.h"
#include "Lightyear/Renderer/Primitives/Shader.h"
#include "Lightyear/Renderer/Primitives/VertexArray.h"

#include "Lightyear/Platform/OpenGL/OpenGLShader.h"

namespace ly::renderer {

Renderer::SceneData Renderer::s_SceneData;

void Renderer::Init() {
    RenderCommand::Init();
}

void Renderer::Shutdown() {}

void Renderer::OnWindowResize(uint32_t width, uint32_t height) {}

void Renderer::BeginScene(const Ref<Camera>& camera) {
    s_SceneData.ViewProjectionMatrix = camera->GetViewProjectionMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const Ref<Shader>& shader,
                      const Ref<VertexArray>& vertexArray,
                      const glm::mat4& transform = glm::mat4(1.f)) {
    // TODO: Need to abstract away openGL call here
    Ref<OpenGLShader> openGLShader = std::dynamic_pointer_cast<OpenGLShader>(shader);
    openGLShader->Use();
    openGLShader->SetUniform("u_ViewProjection", s_SceneData.ViewProjectionMatrix);
    openGLShader->SetUniform("u_Transform", transform);

    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

}  // namespace ly::renderer