#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly::renderer {

class LIGHTYEAR_API UniformBuffer {
public:
    virtual ~UniformBuffer() = default;

    static Ref<UniformBuffer> Create(const CName& name, uint32_t size, uint32_t bindingPoint);

    virtual void Bind() const   = 0;
    virtual void UnBind() const = 0;

    virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

    virtual void Debug(uint32_t programID, const CName& name) = 0;
    [[nodiscard]] virtual const std::string& GetName() const { return m_Name; }

protected:
    std::string m_Name{ "NULL" };
};

}  // namespace ly::renderer