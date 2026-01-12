#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Renderer/Abstract/RendererAPI.h"

namespace ly::renderer {

class OpenGLRendererAPI : public RendererAPI {
public:
    void Init() override;

    void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
    void DrawLines(const VertexArray& vertexArray, uint32_t vertexCount = 0) override;

    void Clear() override;
    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    void SetClearColor(const glm::vec4& color) override;
    void SetLineWidth(float width) override;
};

}  // namespace ly::renderer