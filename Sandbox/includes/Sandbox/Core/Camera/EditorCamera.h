#pragma once

#include "Lightyear.h"

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

    inline void SetSpeed(float speed) { m_Speed = speed; }
    inline float GetSpeed() const { return m_Speed; }

protected:
    float m_Speed{ 1000.f * 100.f };
};