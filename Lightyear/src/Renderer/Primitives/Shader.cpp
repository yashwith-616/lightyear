#include "Lightyear/Renderer/Primitives/Shader.h"
#include "Lightyear/Platform/OpenGL/OpenGLShader.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

#include <glad.h>

namespace ly::renderer {

ly::Ref<Shader> Shader::Create(const std::string& name,
                               const std::unordered_map<ShaderType, std::filesystem::path>& shaderFiles) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            LY_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return MakeRef<OpenGLShader>(name, shaderFiles);
    }

    LY_CORE_ASSERT(false, "Invalid RendererAPI!");
    return nullptr;
}

static ly::Ref<Shader> Create(const std::string& name, const std::unordered_map<ShaderType, std::string>& shaderSrcs) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            LY_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return MakeRef<OpenGLShader>(name, shaderSrcs);
    }

    LY_CORE_ASSERT(false, "Invalid RendererAPI!");
    return nullptr;
}

std::string Shader::ReadFile(std::filesystem::path shaderFilePath) {
    // Open file in binary, and point the pointer to end of file.
    std::ifstream file(shaderFilePath, std::ios::binary | std::ios::ate);
    if (!file) {
        LY_CORE_ASSERT(false, "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: {}", shaderFilePath.string());
    }

    // Get size of file by reading the current position of the file pointer(at the end)
    std::streamsize size = file.tellg();
    // Move the file pointer to the beginning and create a string with exact size
    file.seekg(0, std::ios::beg);
    std::string content(size, '\0');
    file.read(&content[0], size);
    return content;
}

}  // namespace ly::renderer
