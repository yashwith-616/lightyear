#include "Lightyear/Renderer/Primitives/Shader.h"
#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLShader.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

namespace ly::renderer {

Ref<Shader> Shader::Create(const std::string& name,
                           const std::unordered_map<ShaderType, std::filesystem::path>& shaderFiles) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::OpenGL:
            return MakeRef<OpenGLShader>(name, shaderFiles);
        default:
            LY_CORE_ASSERT(false, "Invalid API Type is currently not supported!");
            return nullptr;
    }
}

Ref<Shader> Shader::Create(const std::string& name, const std::unordered_map<ShaderType, std::string>& shaderSrcs) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::OpenGL:
            return MakeRef<OpenGLShader>(name, shaderSrcs);
        default:
            LY_CORE_ASSERT(false, "Invalid API Type is currently not supported!");
            return nullptr;
    }
}

std::string Shader::ReadFile(const std::filesystem::path& shaderFilePath) {
    // Open file in binary, and point the pointer to end of file.
    std::ifstream file(shaderFilePath, std::ios::binary | std::ios::ate);
    if (!file) {
        LY_CORE_ASSERT(false, "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: {}", shaderFilePath.string());
    }

    // Get size of file by reading the current position of the file pointer(at the end)
    const std::streamsize size = file.tellg();
    // Move the file pointer to the beginning and create a string with exact size
    file.seekg(0, std::ios::beg);
    std::string content(size, '\0');
    file.read(&content[0], size);
    return content;
}

}  // namespace ly::renderer
