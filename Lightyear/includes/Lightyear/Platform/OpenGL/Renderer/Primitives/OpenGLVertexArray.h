#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Renderer/Primitives/RenderTypes.h"
#include "Lightyear/Renderer/Primitives/VertexArray.h"

namespace ly::renderer
{

class OpenGLVertexArray : public VertexArray
{
public:
    OpenGLVertexArray();
    ~OpenGLVertexArray() override;

    void bind() const override;
    void unbind() const override;

    void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
    void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

    [[nodiscard]] const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const override { return m_Vertexbuffers; }

    [[nodiscard]] const IndexBuffer& getIndexBuffer() const override
    {
        LY_CORE_ASSERT(m_IndexBuffer, "IndexBuffer is null");
        return *m_IndexBuffer;
    }

private:
    vertexArrayHandle m_RenderID{};
    uint32_t m_VertexBufferIndex{0};

    Ref<IndexBuffer> m_IndexBuffer;
    std::vector<Ref<VertexBuffer>> m_Vertexbuffers;
};

} // namespace ly::renderer
