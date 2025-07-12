#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly::renderer {

class LIGHTYEAR_API Camera {
public:
    const glm::vec3& GetPosition() const { return m_Position; }
    void SetPosition(const glm::vec3& position) { m_Position = position; }

    glm::vec3 GetRotation() const { return m_Rotation; }
    void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; }

    const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

protected:
    glm::mat4 m_ProjectionMatrix{ glm::mat4(1.f) };
    glm::mat4 m_ViewMatrix{ glm::mat4(1.f) };
    glm::mat4 m_ViewProjectionMatrix{ glm::mat4(1.f) };

    glm::vec3 m_Position{};
    glm::vec3 m_Rotation{};

protected:
    virtual void RecalculateViewMatrix()                                                       = 0;
    virtual void RecalculateProjectionMatrix(float left, float right, float bottom, float top) = 0;
};

}  // namespace ly::renderer