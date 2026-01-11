#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly::renderer
{
class Texture
{
public:
    virtual ~Texture() = default;
    [[nodiscard]] virtual uint32_t getHeight() const = 0;
    [[nodiscard]] virtual uint32_t getWidth() const = 0;

    virtual void bind(uint32_t slot = 0) const = 0;
};

class LIGHTYEAR_API Texture2D : public Texture
{
public:
    ~Texture2D() override = default;

    static Ref<Texture2D> create(const CPath& path);
};

} // namespace ly::renderer
