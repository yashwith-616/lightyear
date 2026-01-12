#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly::renderer {

class LIGHTYEAR_API RendererContext {
public:
    virtual ~RendererContext() = default;

    static scope<RendererContext> create(void* windowHandle);

    virtual void init()        = 0;
    virtual void swapBuffers() = 0;
};

}  // namespace ly::renderer