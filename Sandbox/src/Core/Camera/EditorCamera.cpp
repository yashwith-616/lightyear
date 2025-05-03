#include "Sandbox/Core/Camera/EditorCamera.h"

EditorCamera::EditorCamera(float left, float right, float bottom, float top)
    : OrthographicCamera(left, right, bottom, top) {}

void EditorCamera::MoveForward(float value) {
    m_Position.x += value;
    RecalculateViewMatrix();
}

void EditorCamera::MoveUp(float value) {
    m_Position.y += value;
    RecalculateViewMatrix();
}

void EditorCamera::MoveRight(float value) {
    m_Position.z += value;
    RecalculateViewMatrix();
}

void EditorCamera::AddPitch(float value) {}

void EditorCamera::AddYaw(float value) {}

void EditorCamera::AddRoll(float value) {}

void EditorCamera::DebugCamera() {
    LY_LOG(ly::LogType::Trace,
           "Camera postion: {0:.6f}, {1:.6f}, {2:.6f}",
           m_Position.x,
           m_Position.y,
           m_Position.z);
    LY_LOG(ly::LogType::Trace, "Camera Speed: {0:.4f}", m_Speed);
}