#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly::renderer
{

class LIGHTYEAR_API RendererContext
{
public:
    virtual ~RendererContext() = default;

    static Scope<RendererContext> Create(void* windowHandle);

    virtual void Init() = 0;
    virtual void SwapBuffers() = 0;
};

} // namespace ly::renderer
