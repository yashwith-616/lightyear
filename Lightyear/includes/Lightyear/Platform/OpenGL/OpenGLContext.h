#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Renderer/Abstract/RendererContext.h"
#include "lypch.h"

struct GLFWwindow;

namespace ly::renderer {

class LIGHTYEAR_API OpenGLContext : public RendererContext {
public:
    OpenGLContext(GLFWwindow* windowHandle);

    virtual void Init() override;
    virtual void SwapBuffers() override;

private:
    GLFWwindow* m_WindowHandle;
};

}  // namespace ly::renderer