#pragma once

#include "Lightyear/LightyearCore.h"
#include "glm/glm.hpp"

namespace ly::renderer {

class VertexArray;

class LIGHTYEAR_API RendererAPI {
public:
    enum class API { None = 0, OpenGL };

public:
    virtual ~RendererAPI() = default;

    virtual void Init()                                                               = 0;
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void SetClearColor(const glm::vec4& color)                                = 0;
    virtual void Clear()                                                              = 0;

    virtual void DrawIndexed(const VertexArray& vertexArray, uint32_t indexCount = 0) = 0;
    virtual void DrawLines(const VertexArray& vertexArray, uint32_t vertexCount = 0)  = 0;

    virtual void SetLineWidth(float width) = 0;

    static API GetAPI() { return s_API; }
    static RendererAPI* Create();

private:
    static API s_API;
};

}  // namespace ly::renderer