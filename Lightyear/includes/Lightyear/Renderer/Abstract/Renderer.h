#pragma once

#include <lypch.h>
#include "Lightyear/LightyearCore.h"
#include "RendererAPI.h"
#include "glm/glm.hpp"

namespace ly::renderer {

class Camera;
class Shader;
class VertexArray;
class Texture;

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

    static void OnWindowResize(uint32_t width, uint32_t height);

    static void BeginScene(const Ref<Camera>& camera);
    static void EndScene();

    /**
     * @brief Submit the RenderSubmission to Queue to be processed later
     * @param shader the shader
     * @param vertexArray the vertexArray
     * @param texture the texture
     * @param transform the transform
     */
    static void Submit(const RenderSubmission& submission);

    /**
     * @brief Issues all the commands to GPU
     */
    static void Flush();

    static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

private:
    struct SceneData {
        glm::mat4 ViewProjectionMatrix;
    };

    static SceneData s_SceneData;
    static std::vector<RenderSubmission> s_RenderQueue;
};
}  // namespace ly::renderer