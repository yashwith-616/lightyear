#include "Lightyear/Core/Application.h"
#include "Lightyear/Core/Window.h"
#include "Lightyear/Input/Input.h"

LY_DISABLE_WARNINGS_PUSH
#include "GLFW/glfw3.h"
LY_DISABLE_WARNINGS_POP

namespace ly {

bool Input::IsKeyPressed(KeyCode key) {
    auto* window    = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    const int state = glfwGetKey(window, static_cast<int>(key));
    return state == GLFW_PRESS;
}

bool Input::IsMouseButtonPressed(MouseCode button) {
    auto* window    = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    const int state = glfwGetMouseButton(window, static_cast<int>(button));
    return state == GLFW_PRESS;
}

glm::vec2 Input::GetMousePosition() {
    auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    double xpos{};
    double ypos{};
    glfwGetCursorPos(window, &xpos, &ypos);

    return { static_cast<float>(xpos), static_cast<float>(ypos) };
}

float Input::GetMouseX() {
    return GetMousePosition().x;
}

float Input::GetMouseY() {
    return GetMousePosition().y;
}

}  // namespace ly