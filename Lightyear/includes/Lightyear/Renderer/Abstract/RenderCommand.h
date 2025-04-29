#pragma once

#include <lypch.h>
#include <glm/glm.hpp>
#include "Lightyear/LightyearCore.h"

namespace ly::renderer {

class RendererAPI;
class VertexArray;

/**
 * @brief This is a static class which is utilized by the renderer to issue draw calls to the GPU.
 * This class forwards the request directly to the API that is currently configured.
 *
 * RenderCommand can be improved by adding CommandQueue and running the rendering process in a
 * separate thread
 */
class LIGHTYEAR_API RenderCommand {
public:
    inline static void SetClearColor(const glm::vec4& color);

    inline static void Clear();

    inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray);

private:
    static RendererAPI* s_RendererAPI;
};

}  // namespace ly::renderer