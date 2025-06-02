#include "Lightyear/Renderer/Abstract/Renderer.h"
#include "Lightyear/Renderer/Abstract/RenderCommand.h"
#include "Lightyear/Renderer/Abstract/RendererAPI.h"
#include "Lightyear/Renderer/Camera/Camera.h"
#include "Lightyear/Renderer/Primitives/Shader.h"
#include "Lightyear/Renderer/Primitives/Texture.h"
#include "Lightyear/Renderer/Primitives/VertexArray.h"

#include "Lightyear/Platform/OpenGL/OpenGLShader.h"

namespace ly::renderer {

RenderSubmission::RenderSubmission(const Ref<Shader>& shader,
                                   const Ref<VertexArray>& vertexArray,
                                   const Ref<Texture>& texture,
                                   const glm::mat4& transform)
    : RSShader(shader), RSVertexArray(vertexArray), RSTexture(texture), RSTransform(transform) {}

Renderer::SceneData Renderer::s_SceneData;
std::vector<RenderSubmission> Renderer::s_RenderQueue;

void Renderer::Init() {
    RenderCommand::Init();
}

void Renderer::Shutdown() {}

void Renderer::OnWindowResize(uint32_t width, uint32_t height) {}

void Renderer::BeginScene(const Ref<Camera>& camera) {
    s_SceneData.ViewProjectionMatrix = camera->GetViewProjectionMatrix();
    s_RenderQueue.clear();
}

void Renderer::EndScene() {
    Flush();
}

void Renderer::Submit(const RenderSubmission& submission) {
    s_RenderQueue.push_back(submission);
}

void Renderer::Flush() {
    for (const auto& submission : s_RenderQueue) {
        Ref<OpenGLShader> openGLShader =
            std::dynamic_pointer_cast<OpenGLShader>(submission.RSShader);
        openGLShader->Use();
        openGLShader->SetUniform("u_ViewProjection", s_SceneData.ViewProjectionMatrix);
        openGLShader->SetUniform("u_Transform", submission.RSTransform);

        submission.RSTexture->Bind(1);
        openGLShader->SetUniform("u_Color", 1);

        submission.RSVertexArray->Bind();
        RenderCommand::DrawIndexed(submission.RSVertexArray);
    }
}

}  // namespace ly::renderer