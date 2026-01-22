#include "Lightyear/Renderer/Abstract/RendererAPI.h"
#include "Lightyear/Platform/OpenGL/Renderer/Core/OpenGLRendererAPI.h"
#include "Lightyear/Renderer/Primitives/Buffer.h"

namespace ly::renderer
{

scope<RendererApi> RendererApi::create()
{
    switch (m_api)
    {
    case Api::None:
        LY_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        break;
    case Api::OpenGl:
        return scope<OpenGlRendererApi>();
    }

    LY_CORE_ASSERT(false, "Invalid RendererAPI!");
    return nullptr;
}

RendererApi::Api RendererApi::m_api = Api::OpenGl;

} // namespace ly::renderer
