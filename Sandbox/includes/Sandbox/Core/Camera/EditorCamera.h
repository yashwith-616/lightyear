#pragma once

#include "Lightyear.h"

class EditorCamera : public ly::renderer::OrthographicCamera {
public:
    EditorCamera(float left, float right, float bottom, float top);

    virtual void MoveUp(float value);
    virtual void MoveForward(float value);
    virtual void MoveRight(float value);

    virtual void AddPitch(float value);
    virtual void AddYaw(float value);
    virtual void AddRoll(float value);

    virtual void DebugCamera();

    inline void SetSpeed(float speed) { m_Speed = speed; }

    inline float GetSpeed() const { return m_Speed; }

protected:
    float m_Speed{ 1000.f * 100.f };
};