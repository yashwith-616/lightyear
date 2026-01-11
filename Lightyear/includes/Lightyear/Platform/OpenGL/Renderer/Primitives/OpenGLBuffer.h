#pragma once

#include "Lightyear/Renderer/Primitives/Buffer.h"

namespace ly::renderer
{

class LIGHTYEAR_API OpenGLVertexBuffer : public VertexBuffer
{
public:
    explicit OpenGLVertexBuffer(uint32_t size);
    explicit OpenGLVertexBuffer(std::span<const float> vertices);
    ~OpenGLVertexBuffer() override;

    void bind() const override;
    void unBind() const override;
    void setData(const void* data, uint32_t size) override;

    [[nodiscard]] const BufferLayout& getLayout() const override { return m_Layout; }
    void setLayout(const BufferLayout& layout) override { m_Layout = layout; }

private:
    vertexBufferHandle m_RenderID{0};
    BufferLayout m_Layout;
};

class LIGHTYEAR_API OpenGLIndexBuffer : public IndexBuffer
{
public:
    explicit OpenGLIndexBuffer(std::span<const uint32_t> indices);
    ~OpenGLIndexBuffer() override;

    void bind() const override;
    void unBind() const override;
    [[nodiscard]] uint32_t getCount() const override { return m_Count; }

private:
    indexBufferHandle m_RenderID{0};
    uint32_t m_Count{0};
};
} // namespace ly::renderer
