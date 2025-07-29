#pragma once

#include "Lightyear/LightyearCore.h"
#include "RendererAPI.h"
#include "RendererUBO.h"

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
    Ref<Shader> RSShader{};
    Ref<VertexArray> RSVertexArray{};
    Ref<Texture> RSTexture{};
    glm::mat4 RSTransform{};

    RenderSubmission(const Ref<Shader>& shader,
                     const Ref<VertexArray>& vertexArray,
                     const Ref<Texture>& texture,
                     const glm::mat4& transform);
};

class LIGHTYEAR_API Renderer {
public:
    static void Init();
    static void Shutdown();

    static void OnWindowResize(glm::uvec2 size);

    static void BeginScene(const scene::CameraComponent& camera,
                           const scene::TransformComponent& cameraTransform,
                           const scene::SceneData& sceneData);
    static void EndScene();

    /**
     * @brief Submit the RenderSubmission to Queue to be processed later
     * @param submission The render submission
     */
    static void Submit(const RenderSubmission& submission);

    /**
     * @brief Issues all the commands to GPU
     */
    static void Flush();

    static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

private:
    static std::vector<RenderSubmission> s_RenderQueue;
    static GlobalUniforms s_GlobalUniforms;

    static UBO_Camera s_CameraUBO;
    static UBO_Material s_MaterialUBO;
    static UBO_Object s_ObjectUBO;
    static UBO_Scene s_SceneUBO;
};
}  // namespace ly::renderer