#include "Lightyear/Renderer/Primitives/UniformBuffer.h"
#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLUniformBuffer.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

namespace ly::renderer
{

ref<UniformBuffer> UniformBuffer::create(std::string name, uint32_t size, uint32_t bindingPoint)
{
    switch (Renderer::getApi())
    {
    case RendererApi::Api::OpenGl:
        return makeRef<OpenGlUniformBuffer>(std::move(name), size, bindingPoint);
    default:
        LY_CORE_ASSERT(false, "Invalid API Type is currently not supported!");
        return nullptr;
    }

    LY_CORE_ASSERT(false, "Invalid RendererAPI!");
}

} // namespace ly::renderer
