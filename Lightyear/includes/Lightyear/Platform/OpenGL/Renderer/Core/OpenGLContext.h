#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Renderer/Abstract/RendererContext.h"

//NOLINTNEXTLINE
struct GLFWwindow;

namespace ly::renderer {

class LIGHTYEAR_API OpenGlContext : public RendererContext {
public:
    OpenGlContext(GLFWwindow* windowHandle);

    void init() override;
    void swapBuffers() override;

private:
    GLFWwindow* m_windowHandle;
};

}  // namespace ly::renderer