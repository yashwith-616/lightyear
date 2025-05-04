#pragma once

#include "Lightyear/LightyearCore.h"
#include "glm/glm.hpp"

namespace ly::renderer {

class VertexArray;

/**
 * @brief This API is abstraction over DirectX, OpenGL, Vulkan
 * and other specifications. Will not be used by client directly
 *
 * It consits of a static enum API which will be configured by the
 * renderer before the application starts or used.
 *
 * This class will receive direct calls from RenderCommand and will
 * not be called by any other class
 */
class LIGHTYEAR_API RendererAPI {
public:
    enum class API { None = 0, OpenGL };

public:
    virtual ~RendererAPI() = default;

    virtual void Init()                                                               = 0;
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void SetClearColor(const glm::vec4& color)                                = 0;
    virtual void Clear()                                                              = 0;

    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
    virtual void DrawLines(const VertexArray& vertexArray, uint32_t vertexCount = 0)       = 0;

    virtual void SetLineWidth(float width) = 0;

    static API GetAPI() { return s_API; }
    static Scope<RendererAPI> Create();

private:
    static API s_API;
};

}  // namespace ly::renderer