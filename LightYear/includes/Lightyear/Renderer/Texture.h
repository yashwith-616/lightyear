#pragma once

#include <lypch.h>
#include "Lightyear/LightyearCore.h"

namespace ly {

enum class TextureType : uint8_t { DIFFUSE, SPECULAR, NORMAL, HEIGHT };

class LIGHTYEAR_API Texture {
public:
    unsigned int id_;
    TextureType type;
};
}  // namespace ly