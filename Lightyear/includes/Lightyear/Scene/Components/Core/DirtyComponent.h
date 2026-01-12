#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly::scene {

struct LIGHTYEAR_API DirtyComponent {
    bool transform : 1        = true;
    bool cameraView : 1       = true;
    bool cameraProjection : 1 = true;
};

}  // namespace ly::scene