#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly {

/**
 * All the different layers present in the system
 */
enum class SystemLayer : uint8_t { PrePhysics, Physics, PostPhysics, Gameplay, Rendering, Ui, EditorOnly };

}  // namespace ly