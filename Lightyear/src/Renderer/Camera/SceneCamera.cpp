#include "Lightyear/Renderer/Camera/SceneCamera.h"

namespace ly::renderer {

void SceneCamera::Resize(float width, float height) {
    float aspect = width / height;
    RecalculateProjectionMatrix(-aspect * m_Zoom, aspect * m_Zoom, -m_Zoom, m_Zoom);
    RecalculateViewMatrix();
}

}  // namespace ly::renderer