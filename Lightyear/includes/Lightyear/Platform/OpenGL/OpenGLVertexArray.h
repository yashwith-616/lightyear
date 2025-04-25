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

    virtual void AddVertexBuffer(const VertexBuffer& vertexBuffer) override;
    virtual void SetIndexBuffer(const IndexBuffer& indexBuffer) override;

    virtual const std::vector<VertexBuffer>& GetVertexBuffers() const override {
        return m_Vertexbuffers;
    }

    virtual const IndexBuffer& GetIndexBuffer() const override { return *m_IndexBuffer; }

private:
    VertexArrayHandle m_RenderID;
    uint32_t m_VertexBufferIndex{ 0 };
    std::vector<VertexBuffer> m_Vertexbuffers;
    IndexBuffer* m_IndexBuffer;
};

}  // namespace ly::renderer