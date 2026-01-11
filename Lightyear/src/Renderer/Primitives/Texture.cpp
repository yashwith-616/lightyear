#include "Lightyear/Renderer/Primitives/Texture.h"
#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLTexture2D.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

namespace ly::renderer
{

Ref<Texture2D> Texture2D::create(const CPath& path)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::OpenGL:
        return MakeRef<OpenGLTexture2D>(path);
    default:
        LY_CORE_ASSERT(false, "RendererAPI is invalid not supported!");
        return nullptr;
    }
}

} // namespace ly::renderer
