#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Renderer/Primitives/RenderTypes.h"
#include "Lightyear/Renderer/Primitives/VertexArray.h"

namespace ly::renderer {

class OpenGLVertexArray : public VertexArray {
public:
    OpenGLVertexArray();
    ~OpenGLVertexArray() override;

    void Bind() const override;
    void Unbind() const override;

    void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
    void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

    [[nodiscard]] const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_Vertexbuffers; }

    [[nodiscard]] const IndexBuffer& GetIndexBuffer() const override {
        LY_CORE_ASSERT(m_IndexBuffer, "IndexBuffer is null");
        return *m_IndexBuffer;
    }

private:
    VertexArrayHandle m_RenderID{};
    uint32_t m_VertexBufferIndex{ 0 };

    Ref<IndexBuffer> m_IndexBuffer;
    std::vector<Ref<VertexBuffer>> m_Vertexbuffers;
};

}  // namespace ly::renderer