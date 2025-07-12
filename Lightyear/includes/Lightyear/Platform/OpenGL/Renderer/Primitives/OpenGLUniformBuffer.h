#pragma once

#include "Lightyear/Renderer/Primitives/UniformBuffer.h"

namespace ly::renderer {

class OpenGLUniformBuffer : public UniformBuffer {
public:
    OpenGLUniformBuffer(const CName& name, uint32_t size, uint32_t bindingPoint);
    ~OpenGLUniformBuffer() override;

    void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;
    void Bind() const override;
    void UnBind() const override;

    void Debug(uint32_t programID, const CName& name) override;

private:
    uint32_t m_BufferID{ 0 };
    uint32_t m_BindingPoint{ 0 };
};

}  // namespace ly::renderer