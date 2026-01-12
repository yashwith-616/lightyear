#pragma once

#include "Lightyear/Renderer/Primitives/UniformBuffer.h"

namespace ly::renderer {

class OpenGlUniformBuffer : public UniformBuffer {
public:
    OpenGlUniformBuffer(std::string name, uint32_t size, uint32_t bindingPoint);
    ~OpenGlUniformBuffer() override;

    OpenGlUniformBuffer(OpenGlUniformBuffer const&)                = delete;
    OpenGlUniformBuffer& operator=(OpenGlUniformBuffer const&)     = delete;
    OpenGlUniformBuffer(OpenGlUniformBuffer&&) noexcept            = default;
    OpenGlUniformBuffer& operator=(OpenGlUniformBuffer&&) noexcept = default;

    void setData(void const* data, uint32_t size, uint32_t offset = 0) override;
    void bind() const override;
    void unBind() const override;

    void debug(uint32_t programId, std::string const& name) override;

private:
    uint32_t m_bufferId{ 0 };
    uint32_t m_bindingPoint{ 0 };
    uint32_t m_size{ 0 };
};

}  // namespace ly::renderer