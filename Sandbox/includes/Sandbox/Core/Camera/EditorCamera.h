#pragma once

#include "Lightyear.h"

enum class ProjectionType { PERSPECTIVE = 0, ORTHOGRAPHIC = 1, MAX = 2 };

class EditorCamera : public ly::renderer::SceneCamera {
public:
    EditorCamera(float aspectRatio);

    virtual void MoveUp(float value);
    virtual void MoveForward(float value);
    virtual void MoveRight(float value);

    virtual void AddPitch(float value);
    virtual void AddYaw(float value);
    virtual void AddRoll(float value);

    virtual void AddZoom(float value);

    virtual void Resize(float width, float height);

    virtual void DebugCamera();

    inline void SetSpeed(float speed) { m_Speed = std::max(0.f, speed); }
    inline float GetSpeed() const { return m_Speed; }

    void SetPerspective(float fov, float aspectRatio, float nearClip, float farClip);
    void SetOrthographic(float left, float right, float bottom, float top);

protected:
    float m_Speed{ 1.f };
    ProjectionType m_ProjectionType{ ProjectionType::ORTHOGRAPHIC };

protected:
    void SetPerspective();
    void ClampPitch();
};