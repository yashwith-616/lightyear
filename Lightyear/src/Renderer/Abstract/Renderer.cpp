#include "Lightyear/Renderer/Abstract/Renderer.h"

#include "Lightyear/Scene/Components/Camera/CameraComponent.h"
#include "Lightyear/Scene/Components/Transform/TransformComponent.h"

#include "Lightyear/Renderer/Abstract/RenderCommand.h"
#include "Lightyear/Renderer/Primitives/Shader.h"
#include "Lightyear/Renderer/Primitives/Texture.h"
#include "Lightyear/Renderer/Primitives/VertexArray.h"

#include "Lightyear/Scene/SceneData.h"

#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLShader.h"

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

void Renderer::OnWindowResize(glm::uvec2 size) {}

void Renderer::BeginScene(const scene::CameraComponent& camera,
                          const scene::TransformComponent& cameraTransform,
                          const scene::SceneData& sceneData) {
    // Camera UBO
    s_CameraUBO.u_ViewProjection = camera.Cache_ViewProjectionMatrix;
    s_CameraUBO.u_View           = camera.ViewMatrix;
    s_CameraUBO.u_CameraPosition = cameraTransform.Translation;
    s_CameraUBO.u_ZoomLevel      = 1.0f;
    s_GlobalUniforms.UploadCamera(s_CameraUBO);

    // Scene UBO
    s_SceneUBO.u_Time = sceneData.Time;
    s_GlobalUniforms.UploadScene(s_SceneUBO);

    // Material UBO
    s_MaterialUBO.u_BaseColor  = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    s_MaterialUBO.u_Properties = glm::vec4(0.0f);
    s_GlobalUniforms.UploadMaterial(s_MaterialUBO);

    // Reset render queue
    s_RenderQueue.clear();
}

void Renderer::EndScene() {
    Flush();
}

void Renderer::Submit(const RenderSubmission& submission) {
    s_RenderQueue.push_back(submission);
}

void Renderer::Flush() {
    // glDisable(GL_DEPTH_TEST);
    for (const auto& submission : s_RenderQueue) {
        s_ObjectUBO.u_ModelMatrix = submission.RSTransform;
        s_GlobalUniforms.UploadObject(s_ObjectUBO);

        const Ref<OpenGLShader> openGLShader = std::dynamic_pointer_cast<OpenGLShader>(submission.RSShader);
        openGLShader->Use();

        if (submission.RSTexture != nullptr) {
            submission.RSTexture->Bind(1);
            openGLShader->SetUniform("u_Color", 1);
        }

        submission.RSVertexArray->Bind();
        RenderCommand::DrawIndexed(submission.RSVertexArray);

        // s_GlobalUniforms.ObjectUBO->Debug(openGLShader->GetShaderHandle(), "Camera");
    }
}

}  // namespace ly::renderer