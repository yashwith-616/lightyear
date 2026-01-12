#pragma once

#include "Lightyear/LightyearCore.h"

#include "KeyCodes.h"
#include "MouseCodes.h"

namespace ly {

class LIGHTYEAR_API Input {
public:
    static bool isKeyPressed(keyCode key);
    static bool isMouseButtonPressed(mouseCode button);
    static glm::vec2 getMousePosition();
    static float getMouseX();
    static float getMouseY();
};
}  // namespace ly