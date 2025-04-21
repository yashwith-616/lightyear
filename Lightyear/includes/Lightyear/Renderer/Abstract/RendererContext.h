#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly {

class LIGHTYEAR_API RendererContext {
public:
    virtual void Init()        = 0;
    virtual void SwapBuffers() = 0;
};

}  // namespace ly