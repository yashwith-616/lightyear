#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Renderer/Primitives/VertexArray.h"

namespace ly::renderer {

class OpenGLVertexArray : public VertexArray {
public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
    virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

    virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_Vertexbuffers; }

    virtual const IndexBuffer& GetIndexBuffer() const override {
        LY_CORE_ASSERT(m_IndexBuffer, "IndexBuffer is null");
        return *m_IndexBuffer;
    }

private:
    VertexArrayHandle m_RenderID;
    uint32_t m_VertexBufferIndex{ 0 };

    Ref<IndexBuffer> m_IndexBuffer{};
    std::vector<Ref<VertexBuffer>> m_Vertexbuffers{};
};

}  // namespace ly::renderer