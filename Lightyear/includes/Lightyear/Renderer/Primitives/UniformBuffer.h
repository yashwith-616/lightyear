#pragma once

#include <lypch.h>
#include "Lightyear/LightyearCore.h"

namespace ly::renderer {

class LIGHTYEAR_API UniformBuffer {
public:
    virtual ~UniformBuffer()    = default;
    virtual void Bind() const   = 0;
    virtual void UnBind() const = 0;

    virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;
    virtual CName GetName() const { return m_Name; }

    static ly::Ref<UniformBuffer> Create(const CName& name, uint32_t size, uint32_t bindingPoint);

protected:
    CName m_Name{};
};

}  // namespace ly::renderer