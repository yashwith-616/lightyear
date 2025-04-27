#pragma once

#include "RendererAPI.h"
#include "glm/glm.hpp"

namespace ly::renderer {

class VertexArray;

class Renderer {
public:
    static void Init();
    static void Shutdown();

    static void OnWindowResize(uint32_t width, uint32_t height);

    static void BeginScene();
    static void EndScene();

    static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

    static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

private:
    struct SceneData {
        glm::mat4 ViewProjectionMatrix;
    };

    static SceneData s_SceneData;
};
}  // namespace ly::renderer