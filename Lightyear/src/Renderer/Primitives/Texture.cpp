#include "Lightyear/Renderer/Primitives/Texture.h"
#include "Lightyear/Platform/OpenGL/OpenGLTexture2D.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

namespace ly::renderer {

Ref<Texture2D> Texture2D::Create(const CPath& path) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            LY_CORE_ASSERT(false, "RenderereAPI::None is currently not supported!");
        case RendererAPI::API::OpenGL:
            return MakeRef<OpenGLTexture2D>(path);
    }
}

}  // namespace ly::renderer