#pragma once

#include "Lightyear/Renderer/Primitives/Buffer.h"

namespace ly::renderer {

class LIGHTYEAR_API OpenGLVertexBuffer : public VertexBuffer {
public:
    explicit OpenGLVertexBuffer(uint32_t size);
    OpenGLVertexBuffer(const float* vertices, uint32_t size);
    ~OpenGLVertexBuffer() override;

    void Bind() const override;
    void UnBind() const override;
    void SetData(const void* data, uint32_t size) override;
    const BufferLayout& GetLayout() const override { return m_Layout; }
    void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

private:
    VertexBufferHandle m_RenderID{ 0 };
    BufferLayout m_Layout;
};

class LIGHTYEAR_API OpenGLIndexBuffer : public IndexBuffer {
public:
    OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
    ~OpenGLIndexBuffer() override;

    void Bind() const override;
    void UnBind() const override;
    uint32_t GetCount() const override { return m_Count; }

private:
    IndexBufferHandle m_RenderID{ 0 };
    uint32_t m_Count{ 0 };
};
}  // namespace ly::renderer