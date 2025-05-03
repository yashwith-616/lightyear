#pragma once

#include "Camera.h"

namespace ly::renderer {

class LIGHTYEAR_API OrthographicCamera : public Camera {
public:
    OrthographicCamera(float left, float right, float bottom, float top);

protected:
    virtual void RecalculateViewMatrix();
};

}  // namespace ly::renderer