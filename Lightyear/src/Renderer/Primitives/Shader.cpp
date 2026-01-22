#include "Lightyear/Renderer/Primitives/Shader.h"
#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLShader.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

namespace ly::renderer
{

ref<Shader>
    Shader::create(std::string const& name, std::unordered_map<ShaderType, std::filesystem::path> const& shaderFiles)
{
    switch (Renderer::getApi())
    {
    case RendererApi::Api::OpenGl:
        return makeRef<OpenGlShader>(name, shaderFiles);
    default:
        LY_CORE_ASSERT(false, "Invalid API Type is currently not supported!");
        return nullptr;
    }
}

ref<Shader> Shader::create(std::string const& name, std::unordered_map<ShaderType, std::string> const& shaderSrcs)
{
    switch (Renderer::getApi())
    {
    case RendererApi::Api::OpenGl:
        return makeRef<OpenGlShader>(name, shaderSrcs);
    default:
        LY_CORE_ASSERT(false, "Invalid API Type is currently not supported!");
        return nullptr;
    }
}

std::string Shader::readFile(std::filesystem::path const& shaderFilePath)
{
    // Open file in binary, and point the pointer to end of file.
    std::ifstream file(shaderFilePath, std::ios::binary | std::ios::ate);
    if (!file)
    {
        LY_CORE_ASSERT(false, "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: {}", shaderFilePath.string());
    }

    // Get size of file by reading the current position of the file pointer(at the end)
    std::streamsize const size = file.tellg();
    // Move the file pointer to the beginning and create a string with exact size
    file.seekg(0, std::ios::beg);
    std::string content(size, '\0');
    file.read(content.data(), size);
    return content;
}

} // namespace ly::renderer
