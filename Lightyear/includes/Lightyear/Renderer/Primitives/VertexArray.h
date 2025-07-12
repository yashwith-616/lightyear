#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly::renderer {

class VertexBuffer;
class IndexBuffer;

class LIGHTYEAR_API VertexArray {
public:
    virtual ~VertexArray() = default;

    static Ref<VertexArray> Create();

    virtual void Bind() const   = 0;
    virtual void Unbind() const = 0;

    virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
    virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)    = 0;

    [[nodiscard]] virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
    [[nodiscard]] virtual const IndexBuffer& GetIndexBuffer() const                      = 0;
};

}  // namespace ly::renderer