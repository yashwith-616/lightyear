#include "Lightyear/Renderer/Primitives/UniformBuffer.h"
#include "Lightyear/Platform/OpenGL/OpenGLUniformBuffer.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

namespace ly::renderer {

ly::Ref<UniformBuffer> UniformBuffer::Create(const CName& name,
                                             uint32_t size,
                                             uint32_t bindingPoint) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            LY_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return MakeRef<OpenGLUniformBuffer>(name, size, bindingPoint);
    }

    LY_CORE_ASSERT(false, "Invalid RendererAPI!");
    return nullptr;
}

}  // namespace ly::renderer