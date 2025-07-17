#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly::renderer {

class LIGHTYEAR_API RendererContext {
public:
    static Scope<RendererContext> Create(void* windowHandle);
    virtual ~RendererContext() = default;

    virtual void Init()        = 0;
    virtual void SwapBuffers() = 0;
};

}  // namespace ly::renderer