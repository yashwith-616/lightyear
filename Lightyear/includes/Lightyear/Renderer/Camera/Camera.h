#pragma once

#include "Lightyear/LightyearCore.h"
#include "glm/glm.hpp"

namespace ly::renderer {

class LIGHTYEAR_API Camera {
public:
    const glm::vec3& GetPosition() const { return m_Position; }
    void SetPosition(const glm::vec3& position) { m_Position = position; }

    float GetRotation() const { return m_Rotation; }
    void SetRotation(float rotation) { m_Rotation = rotation; }

    const glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }
    const glm::mat4& GetViewMatrix() { return m_ViewMatrix; }
    const glm::mat4& GetViewProjectionMatrix() { return m_ViewProjectionMatrix; }

protected:
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ViewProjectionMatrix;

    glm::vec3 m_Position;
    float m_Rotation = 0.f;

protected:
    void RecalculateViewMatrix() = 0;
};

}  // namespace ly::renderer