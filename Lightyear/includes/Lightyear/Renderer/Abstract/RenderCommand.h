#pragma once

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
    static void init() { m_rendererApi->init(); }

    static void setClearColor(glm::vec4 const& color) { m_rendererApi->setClearColor(color); }

    static void clear() { m_rendererApi->clear(); }

    static void drawIndexed(ref<VertexArray> const& vertexArray) { m_rendererApi->drawIndexed(vertexArray); }

private:
    static scope<RendererApi> m_rendererApi;
};

}  // namespace ly::renderer