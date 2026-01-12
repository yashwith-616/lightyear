#include "Lightyear/Renderer/Primitives/UniformBuffer.h"
#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLUniformBuffer.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

namespace ly::renderer {

Ref<UniformBuffer> UniformBuffer::Create(std::string name, uint32_t size, uint32_t bindingPoint) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::OpenGL:
            return MakeRef<OpenGLUniformBuffer>(std::move(name), size, bindingPoint);
        default:
            LY_CORE_ASSERT(false, "Invalid API Type is currently not supported!");
            return nullptr;
    }

    LY_CORE_ASSERT(false, "Invalid RendererAPI!");
}

}  // namespace ly::renderer