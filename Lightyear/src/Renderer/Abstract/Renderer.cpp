#include "Lightyear/Renderer/Abstract/Renderer.h"
#include "Lightyear/Renderer/Abstract/RenderCommand.h"
#include "Lightyear/Renderer/Abstract/RendererAPI.h"
#include "Lightyear/Renderer/Camera/Camera.h"
#include "Lightyear/Renderer/Primitives/Shader.h"
#include "Lightyear/Renderer/Primitives/Texture.h"
#include "Lightyear/Renderer/Primitives/VertexArray.h"

#include "Lightyear/Scene/SceneData.h"

#include "Lightyear/Platform/OpenGL/OpenGLShader.h"

namespace ly::renderer {

GlobalUniforms Renderer::s_GlobalUniforms;
UBO_Camera Renderer::s_CameraUBO;
UBO_Material Renderer::s_MaterialUBO;
UBO_Object Renderer::s_ObjectUBO;
UBO_Scene Renderer::s_SceneUBO;

std::vector<RenderSubmission> Renderer::s_RenderQueue;

RenderSubmission::RenderSubmission(const Ref<Shader>& shader,
                                   const Ref<VertexArray>& vertexArray,
                                   const Ref<Texture>& texture,
                                   const glm::mat4& transform)
    : RSShader(shader), RSVertexArray(vertexArray), RSTexture(texture), RSTransform(transform) {}

void Renderer::Init() {
    RenderCommand::Init();
    s_GlobalUniforms.Init();
    s_GlobalUniforms.Bind();
}

void Renderer::Shutdown() {}

void Renderer::OnWindowResize(uint32_t width, uint32_t height) {}

void Renderer::BeginScene(const Ref<Camera>& camera, const scene::SceneData& sceneData) {
    s_CameraUBO.ViewProjection = camera->GetViewProjectionMatrix();
    s_CameraUBO.View           = camera->GetViewMatrix();
    s_CameraUBO.CameraPosition = camera->GetPosition();
    s_CameraUBO.CameraRotation = camera->GetRotation();
    s_GlobalUniforms.UploadCamera(s_CameraUBO);

    s_SceneUBO.Time = sceneData.Time;
    s_GlobalUniforms.UploadScene(s_SceneUBO);

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
        s_ObjectUBO.ModelMatrix = submission.RSTransform;
        s_GlobalUniforms.UploadObject(s_ObjectUBO);

        Ref<OpenGLShader> openGLShader =
            std::dynamic_pointer_cast<OpenGLShader>(submission.RSShader);
        openGLShader->Use();
        submission.RSTexture->Bind(1);
        openGLShader->SetUniform("u_Color", 1);

        submission.RSVertexArray->Bind();
        RenderCommand::DrawIndexed(submission.RSVertexArray);

        // s_GlobalUniforms.CameraUBO->Debug(openGLShader->GetShaderHandle(), "Scene");
    }
}

}  // namespace ly::renderer