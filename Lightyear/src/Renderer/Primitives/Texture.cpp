#include "Lightyear/Renderer/Primitives/Texture.h"
#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLTexture2D.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

namespace ly::renderer
{

ref<Texture2D> Texture2D::create(cPath const& path)
{
    switch (Renderer::getApi())
    {
    case RendererApi::Api::OpenGl:
        return makeRef<OpenGlTexture2D>(path);
    default:
        LY_CORE_ASSERT(false, "RendererAPI is invalid not supported!");
        return nullptr;
    }
}

} // namespace ly::renderer
