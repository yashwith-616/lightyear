#include "Lightyear/Renderer/Abstract/RenderCommand.h"
#include "Lightyear/Platform/OpenGL/OpenGLRendererAPI.h"

namespace ly::renderer {

Scope<RendererAPI> RenderCommand::s_RendererAPI = MakeScope<OpenGLRendererAPI>();

}  // namespace ly::renderer
