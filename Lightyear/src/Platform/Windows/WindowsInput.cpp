#include "Lightyear/Core/Application.h"
#include "Lightyear/Core/Window.h"
#include "Lightyear/Input/Input.h"

LY_DISABLE_WARNINGS_PUSH
#include "GLFW/glfw3.h"
LY_DISABLE_WARNINGS_POP

namespace ly
{

bool Input::isKeyPressed(keyCode key)
{
    auto* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
    int const state = glfwGetKey(window, static_cast<int>(key));
    return state == GLFW_PRESS;
}

bool Input::isMouseButtonPressed(mouseCode button)
{
    auto* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
    int const state = glfwGetMouseButton(window, static_cast<int>(button));
    return state == GLFW_PRESS;
}

glm::vec2 Input::getMousePosition()
{
    auto* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
    double xpos{};
    double ypos{};
    glfwGetCursorPos(window, &xpos, &ypos);

    return {static_cast<float>(xpos), static_cast<float>(ypos)};
}

float Input::getMouseX() { return getMousePosition().x; }

float Input::getMouseY() { return getMousePosition().y; }

} // namespace ly
