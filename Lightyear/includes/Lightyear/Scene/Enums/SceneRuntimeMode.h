#pragma once
#include "Lightyear/pch/lypch.h"

namespace ly::scene {

enum class SceneRuntimeMode : uint8_t { None = 0, Pause, Play, Simulation, Edit, Max };

}  // namespace ly::scene