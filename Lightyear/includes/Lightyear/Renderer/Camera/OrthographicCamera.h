#pragma once

#include "Camera.h"

namespace ly::renderer {

class OrthographicCamera : public Camera {
public:
    OrthographicCamera(float left, float right, float bottom, float top);

protected:
    virtual void RecalculateViewMatrix();
};

}  // namespace ly::renderer