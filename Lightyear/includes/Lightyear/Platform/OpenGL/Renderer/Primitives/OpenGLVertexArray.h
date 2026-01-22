#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Renderer/Primitives/RenderTypes.h"
#include "Lightyear/Renderer/Primitives/VertexArray.h"

namespace ly::renderer
{

class OpenGlVertexArray : public VertexArray
{
public:
    OpenGlVertexArray();
    ~OpenGlVertexArray() override;

    void bind() const override;
    void unbind() const override;

    void addVertexBuffer(ref<VertexBuffer> const& vertexBuffer) override;
    void setIndexBuffer(ref<IndexBuffer> const& indexBuffer) override;

    [[nodiscard]] std::vector<ref<VertexBuffer>> const& getVertexBuffers() const override { return m_vertexbuffers; }

    [[nodiscard]] IndexBuffer const& getIndexBuffer() const override
    {
        LY_CORE_ASSERT(m_indexBuffer, "IndexBuffer is null");
        return *m_indexBuffer;
    }

private:
    vertexArrayHandle m_renderId{};
    uint32_t m_vertexBufferIndex{0};

    ref<IndexBuffer> m_indexBuffer;
    std::vector<ref<VertexBuffer>> m_vertexbuffers;
};

} // namespace ly::renderer
