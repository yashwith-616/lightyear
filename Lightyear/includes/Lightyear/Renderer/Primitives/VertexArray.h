#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly::renderer
{

class VertexBuffer;
class IndexBuffer;

class LIGHTYEAR_API VertexArray
{
public:
    virtual ~VertexArray() = default;

    static Ref<VertexArray> create();

    virtual void bind() const;
    virtual void unbind() const = 0;

    virtual void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
    virtual void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

    [[nodiscard]] virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const = 0;
    [[nodiscard]] virtual const IndexBuffer& getIndexBuffer() const = 0;
};

} // namespace ly::renderer
