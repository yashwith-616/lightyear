#include "Sandbox/Core/Camera/EditorCamera.h"

EditorCamera::EditorCamera(float aspectRatio) : SceneCamera(aspectRatio) {}

void EditorCamera::Resize(float width, float height) {
    float aspect = width / height;
    RecalculateProjectionMatrix(-aspect * m_Zoom, aspect * m_Zoom, -m_Zoom, m_Zoom);
    RecalculateViewMatrix();
}

void EditorCamera::MoveForward(float value) {
    m_Position.z += value;
    RecalculateViewMatrix();
}

void EditorCamera::MoveUp(float value) {
    m_Position.y += value;
    RecalculateViewMatrix();
}

void EditorCamera::MoveRight(float value) {
    m_Position.x += value;
    RecalculateViewMatrix();
}

void EditorCamera::AddPitch(float value) {
    m_Rotation.x += value;
    ClampPitch();
    RecalculateViewMatrix();
}

void EditorCamera::AddYaw(float value) {
    m_Rotation.y += value;
    RecalculateViewMatrix();
}

void EditorCamera::AddRoll(float value) {
    m_Rotation.z += value;
    RecalculateViewMatrix();
}

void EditorCamera::AddZoom(float value) {
    m_Zoom += value;
}

void EditorCamera::SetPerspective(float fov, float aspectRatio, float nearClip, float farClip) {
    m_ProjectionMatrix     = glm::perspective(fov, aspectRatio, nearClip, farClip);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

    RecalculateViewMatrix();
    m_ProjectionType = ProjectionType::PERSPECTIVE;
}

void EditorCamera::SetOrthographic(float left, float right, float bottom, float top) {
    m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    RecalculateViewMatrix();
}

void EditorCamera::ClampPitch() {
    // Prevent camera flipping if pitch goes beyond straight up/down
    const float limit = glm::radians(89.0f);
    if (m_Rotation.x > limit)
        m_Rotation.x = limit;
    else if (m_Rotation.x < -limit)
        m_Rotation.x = -limit;
}

void EditorCamera::DebugCamera() {
    // LY_LOG(ly::LogType::Trace, "Camera postion: {0:.6f}, {1:.6f}, {2:.6f}", m_Position);
    LY_LOG(ly::LogType::Trace, "Camera Speed: {0:.4f}", m_Speed);
}