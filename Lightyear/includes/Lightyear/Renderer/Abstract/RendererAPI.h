#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly::renderer {

class VertexArray;

/**
 * @brief This API is abstraction over DirectX, OpenGL, Vulkan
 * and other specifications. Will not be used by client directly
 *
 * It consists of a static enum API which will be configured by the
 * renderer before the application starts or used.
 *
 * This class will receive direct calls from RenderCommand and will
 * not be called by any other class
 */
class LIGHTYEAR_API RendererApi {
public:
    enum class Api { None = 0, OpenGl };

    virtual ~RendererApi() = default;

    virtual void init()                                                               = 0;
    virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void setClearColor(glm::vec4 const& color)                                = 0;
    virtual void clear()                                                              = 0;

    virtual void drawIndexed(ref<VertexArray> const& vertexArray, uint32_t indexCount = 0) = 0;
    virtual void drawLines(VertexArray const& vertexArray, uint32_t vertexCount = 0)       = 0;

    virtual void setLineWidth(float width) = 0;

    static Api getApi() { return m_api; }
    static scope<RendererApi> create();

private:
    static Api m_api;
};

}  // namespace ly::renderer