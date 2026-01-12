#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly::renderer {

class LIGHTYEAR_API UniformBuffer {
public:
    explicit UniformBuffer(std::string name) : m_name{ std::move(name) } {}
    virtual ~UniformBuffer() = default;

    static ref<UniformBuffer> create(std::string name, uint32_t size, uint32_t bindingPoint);

    virtual void bind() const   = 0;
    virtual void unBind() const = 0;

    virtual void setData(void const* data, uint32_t size, uint32_t offset = 0) = 0;

    virtual void debug(uint32_t programId, std::string const& name) = 0;
    [[nodiscard]] virtual std::string const& getName() const { return m_name; }

protected:
    std::string m_name{ k_kNotset };
};

}  // namespace ly::renderer