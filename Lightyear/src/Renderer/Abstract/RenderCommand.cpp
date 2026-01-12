#include "Lightyear/Renderer/Abstract/RenderCommand.h"
#include "Lightyear/Platform/OpenGL/Renderer/Core/OpenGLRendererAPI.h"

namespace ly::renderer {

scope<RendererApi> RenderCommand::m_rendererApi = makeScope<OpenGlRendererApi>();

}  // namespace ly::renderer
