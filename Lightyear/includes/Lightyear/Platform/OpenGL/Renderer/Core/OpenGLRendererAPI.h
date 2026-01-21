#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Renderer/Abstract/RendererAPI.h"

namespace ly::renderer
{

class OpenGlRendererApi : public RendererApi
{
public:
    void init() override;

    void drawIndexed(ref<VertexArray> const& vertexArray, uint32_t indexCount = 0) override;
    void drawLines(VertexArray const& vertexArray, uint32_t vertexCount = 0) override;

    void clear() override;
    void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    void setClearColor(glm::vec4 const& color) override;
    void setLineWidth(float width) override;
};

} // namespace ly::renderer
