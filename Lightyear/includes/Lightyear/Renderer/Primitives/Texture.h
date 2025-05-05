#pragma once

#include "Lightyear/LightyearCore.h"
#include "RenderTypes.h"

namespace ly::renderer {
class Texture {
public:
    virtual uint32_t GetHeight() const = 0;
    virtual uint32_t GetWidth() const  = 0;

    virtual void Bind(uint32_t slot = 0) const = 0;
};

class LIGHTYEAR_API Texture2D : public Texture {
public:
    virtual ~Texture2D() = default;

    static Ref<Texture2D> Create(const CPath& path);
};

}  // namespace ly::renderer