#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly::renderer {

class VertexBuffer;
class IndexBuffer;

class LIGHTYEAR_API VertexArray {
public:
    virtual ~VertexArray() = default;

    static ref<VertexArray> create();

    virtual void bind() const = 0;
    virtual void unbind() const;

    virtual void addVertexBuffer(ref<VertexBuffer> const& vertexBuffer);
    virtual void setIndexBuffer(ref<IndexBuffer> const& indexBuffer);

    [[nodiscard]] virtual std::vector<ref<VertexBuffer>> const& getVertexBuffers() const = 0;
    [[nodiscard]] virtual IndexBuffer const& getIndexBuffer() const                      = 0;
};

}  // namespace ly::renderer