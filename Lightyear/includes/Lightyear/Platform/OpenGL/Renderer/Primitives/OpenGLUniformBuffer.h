#pragma once

#include "Lightyear/Renderer/Primitives/UniformBuffer.h"

namespace ly::renderer
{

class OpenGLUniformBuffer : public UniformBuffer
{
public:
    OpenGLUniformBuffer(std::string name, uint32_t size, uint32_t bindingPoint);
    ~OpenGLUniformBuffer() override;

    OpenGLUniformBuffer(const OpenGLUniformBuffer&) = delete;
    OpenGLUniformBuffer& operator=(const OpenGLUniformBuffer&) = delete;
    OpenGLUniformBuffer(OpenGLUniformBuffer&&) noexcept = default;
    OpenGLUniformBuffer& operator=(OpenGLUniformBuffer&&) noexcept = default;

    void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;
    void Bind() const override;
    void UnBind() const override;

    void Debug(uint32_t programID, const std::string& name) override;

private:
    uint32_t m_BufferID{0};
    uint32_t m_BindingPoint{0};
    uint32_t m_Size{0};
};

} // namespace ly::renderer
