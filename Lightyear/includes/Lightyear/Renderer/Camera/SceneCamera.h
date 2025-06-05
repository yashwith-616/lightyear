#pragma once

#include "Lightyear/LightyearCore.h"
#include "OrthographicCamera.h"

namespace ly::renderer {

class LIGHTYEAR_API SceneCamera : public OrthographicCamera {
public:
    SceneCamera(float aspectRatio) : OrthographicCamera(-aspectRatio, aspectRatio, -1, 1) {}

    void Resize(float width, float height);

protected:
    float m_Zoom{ 1.f };
};
}  // namespace ly::renderer