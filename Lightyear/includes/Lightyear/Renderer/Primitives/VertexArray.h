#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Renderer/Primitives/RenderTypes.h"

namespace ly::renderer {

class VertexBuffer;
class IndexBuffer;

class LIGHTYEAR_API VertexArray {
public:
    virtual ~VertexArray() = default;

    virtual void Bind() const   = 0;
    virtual void Unbind() const = 0;

    virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
    virtual void SetIndexBuffer(const IndexBuffer* indexBuffer)                     = 0;

    virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
    virtual const IndexBuffer& GetIndexBuffer() const                                  = 0;

    static VertexArray* Create();
};

}  // namespace ly::renderer