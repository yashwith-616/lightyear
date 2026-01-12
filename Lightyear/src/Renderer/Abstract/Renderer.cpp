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

RenderSubmission::RenderSubmission(ref<Shader> const& shader,
                                   ref<VertexArray> const& vertexArray,
                                   ref<Texture> const& texture,
                                   glm::mat4 const& transform)
    : rsShader(shader), rsVertexArray(vertexArray), rsTexture(texture), rsTransform(transform) {}
void Renderer::init() {
    RenderCommand::init();
    m_sGlobalUniforms.init();
    m_sGlobalUniforms.bind();
}
void Renderer::shutdown() {}
void Renderer::onWindowResize(glm::uvec2 size) {}
void Renderer::beginScene(scene::CameraComponent const& camera,
                          scene::TransformComponent const& cameraTransform,
                          scene::SceneData const& sceneData) {
    // Camera UBO
    m_sCameraUbo.uViewProjection = camera.cacheViewProjectionMatrix;
    m_sCameraUbo.uView           = camera.viewMatrix;
    m_sCameraUbo.uCameraPosition = cameraTransform.translation;
    m_sCameraUbo.uZoomLevel      = 1.0f;
    m_sGlobalUniforms.uploadCamera(m_sCameraUbo);

    // Scene UBO
    m_sSceneUbo.uTime = sceneData.time;
    m_sGlobalUniforms.uploadScene(m_sSceneUbo);

    // Material UBO
    m_sMaterialUbo.uBaseColor  = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    m_sMaterialUbo.uProperties = glm::vec4(0.0f);
    m_sGlobalUniforms.uploadMaterial(m_sMaterialUbo);

    // Reset render queue
    m_sRenderQueue.clear();
}

void Renderer::endScene() {
    flush();
}

void Renderer::submit(RenderSubmission const& submission) {
    m_sRenderQueue.push_back(submission);
}

void Renderer::flush() {
    for (auto const& submission : m_sRenderQueue) {
        m_sObjectUbo.uModelMatrix = submission.rsTransform;
        m_sGlobalUniforms.uploadObject(m_sObjectUbo);

        ref<OpenGlShader> const openGlShader = std::dynamic_pointer_cast<OpenGlShader>(submission.rsShader);
        openGlShader->use();

        if (submission.rsTexture != nullptr) {
            submission.rsTexture->bind(1);
            openGlShader->setUniform("u_Color", 1);
        }

        submission.rsVertexArray->bind();
        RenderCommand::drawIndexed(submission.rsVertexArray);
    }
}

std::vector<RenderSubmission> Renderer::m_sRenderQueue;

GlobalUniforms Renderer::m_sGlobalUniforms;

UboCamera Renderer::m_sCameraUbo;

UboMaterial Renderer::m_sMaterialUbo;

UboObject Renderer::m_sObjectUbo;

UboScene Renderer::m_sSceneUbo;

}  // namespace ly::renderer