#pragma once

#include <Lightyear/Renderer/Abstract/RendererAPI.h>
#include <Lightyear/Renderer/Abstract/RendererUBO.h>
#include "Lightyear/LightyearCore.h"

namespace ly::scene {
struct SceneData;
struct CameraComponent;
struct TransformComponent;
}  // namespace ly::scene

namespace ly::renderer {

class Shader;
class VertexArray;
class Texture;

/**
 * Fix this entire class when Rendering System is prioritized and written
 *
 */
struct LIGHTYEAR_API RenderSubmission {
    ref<Shader> rsShader{};
    ref<VertexArray> rsVertexArray{};
    ref<Texture> rsTexture{};
    glm::mat4 rsTransform{};

    RenderSubmission(ref<Shader> const& shader,
                     ref<VertexArray> const& vertexArray,
                     ref<Texture> const& texture,
                     glm::mat4 const& transform);
};

class LIGHTYEAR_API Renderer {
public:
    static void init();
    static void shutdown();

    static void onWindowResize(glm::uvec2 size);

    static void beginScene(scene::CameraComponent const& camera,
                           scene::TransformComponent const& cameraTransform,
                           scene::SceneData const& sceneData);
    static void endScene();

    /**
     * @brief Submit the RenderSubmission to Queue to be processed later
     * @param submission The render submission
     */
    static void submit(RenderSubmission const& submission);

    /**
     * @brief Issues all the commands to GPU
     */
    static void flush();

    static RendererApi::Api getApi() { return RendererApi::getApi(); }

private:
    static std::vector<RenderSubmission> m_sRenderQueue;
    static GlobalUniforms m_sGlobalUniforms;

    static UboCamera m_sCameraUbo;
    static UboMaterial m_sMaterialUbo;
    static UboObject m_sObjectUbo;
    static UboScene m_sSceneUbo;
};

}  // namespace ly::renderer