#pragma once

#include "Lightyear/LightyearCore.h"

#include "KeyCodes.h"
#include "MouseCodes.h"

namespace ly
{

class LIGHTYEAR_API Input
{
public:
    static bool IsKeyPressed(KeyCode key);
    static bool IsMouseButtonPressed(MouseCode button);
    static glm::vec2 GetMousePosition();
    static float GetMouseX();
    static float GetMouseY();
};
} // namespace ly
