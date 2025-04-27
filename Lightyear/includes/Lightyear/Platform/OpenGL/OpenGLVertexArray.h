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

    virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
    virtual void SetIndexBuffer(const IndexBuffer* indexBuffer) override;

    virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override {
        return m_Vertexbuffers;
    }

    virtual const IndexBuffer& GetIndexBuffer() const override { return *m_IndexBuffer; }

private:
    VertexArrayHandle m_RenderID;
    uint32_t m_VertexBufferIndex{ 0 };
    std::vector<std::shared_ptr<VertexBuffer>> m_Vertexbuffers;
    IndexBuffer* m_IndexBuffer{ nullptr };
};

}  // namespace ly::renderer