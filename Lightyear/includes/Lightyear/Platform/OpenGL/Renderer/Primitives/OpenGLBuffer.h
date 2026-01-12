#pragma once

#include "Lightyear/Renderer/Primitives/Buffer.h"

namespace ly::renderer {

class LIGHTYEAR_API OpenGlVertexBuffer : public VertexBuffer {
public:
    explicit OpenGlVertexBuffer(uint32_t size);
    explicit OpenGlVertexBuffer(std::span<float const> vertices);
    ~OpenGlVertexBuffer() override;

    void bind() const override;
    void unBind() const override;
    void setData(void const* data, uint32_t size) override;

    [[nodiscard]] BufferLayout const& getLayout() const override { return m_layout; }
    void setLayout(BufferLayout const& layout) override { m_layout = layout; }

private:
    vertexBufferHandle m_renderId{ 0 };
    BufferLayout m_layout;
};

class LIGHTYEAR_API OpenGlIndexBuffer : public IndexBuffer {
public:
    explicit OpenGlIndexBuffer(std::span<uint32_t const> indices);
    ~OpenGlIndexBuffer() override;

    void bind() const override;
    void unBind() const override;
    [[nodiscard]] uint32_t getCount() const override { return m_count; }

private:
    indexBufferHandle m_renderId{ 0 };
    uint32_t m_count{ 0 };
};
}  // namespace ly::renderer