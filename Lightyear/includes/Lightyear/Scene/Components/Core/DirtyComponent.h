#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly::scene {

struct LIGHTYEAR_API DirtyComponent {
    bool Transform : 1         = true;
    bool Camera_View : 1       = true;
    bool Camera_Projection : 1 = true;
};

}  // namespace ly::scene