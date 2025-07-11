#pragma once

#include <glm/glm.hpp>
#include "Lightyear/LightyearCore.h"
#include "Lightyear/Renderer/Abstract/RendererAPI.h"
#include "Lightyear/Renderer/Primitives/VertexArray.h"

namespace ly::renderer {

/**
 * @brief This is a static class which is utilized by the renderer to issue draw calls to the GPU.
 * This class forwards the request directly to the API that is currently configured.
 *
 * RenderCommand can be improved by adding CommandQueue and running the rendering process in a
 * separate thread
 */
class LIGHTYEAR_API RenderCommand {
public:
    inline static void Init() { s_RendererAPI->Init(); }

    inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }

    inline static void Clear() { s_RendererAPI->Clear(); }

    inline static void DrawIndexed(const Ref<VertexArray>& vertexArray) { s_RendererAPI->DrawIndexed(vertexArray); }

private:
    static Scope<RendererAPI> s_RendererAPI;
};

}  // namespace ly::renderer