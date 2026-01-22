#pragma once

#include <Lightyear/Renderer/Primitives/RenderTypes.h>
#include "Lightyear/LightyearCore.h"

namespace ly::renderer
{
class Texture
{
public:
    virtual uint32_t getHeight() const = 0;
    virtual uint32_t getWidth() const = 0;

    virtual void bind(uint32_t slot = 0) const = 0;
};

class LIGHTYEAR_API Texture2D : public Texture
{
public:
    virtual ~Texture2D() = default;

    static ref<Texture2D> create(cPath const& path);
};

} // namespace ly::renderer
