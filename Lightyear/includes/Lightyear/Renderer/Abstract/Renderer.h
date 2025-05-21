#pragma once

#include "Lightyear/LightyearCore.h"
#include "RendererAPI.h"
#include "glm/glm.hpp"

namespace ly::renderer {

class Camera;
class Shader;
class VertexArray;
class Texture;

class LIGHTYEAR_API Renderer {
public:
    static void Init();
    static void Shutdown();

    static void OnWindowResize(uint32_t width, uint32_t height);

    static void BeginScene(const Ref<Camera>& camera);
    static void EndScene();

    static void Submit(const Ref<Shader>& shader,
                       const Ref<VertexArray>& vertexArray,
                       const Ref<Texture>& texture,
                       const glm::mat4& transform);

    static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

private:
    struct SceneData {
        glm::mat4 ViewProjectionMatrix;
    };

    static SceneData s_SceneData;
};
}  // namespace ly::renderer