#include "Lightyear/Renderer/Primitives/Shader.h"
#include <glad.h>
#include "Lightyear/Platform/OpenGL/OpenGLShader.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

namespace ly::renderer {

ly::Ref<Shader> Shader::Create(std::string_view vertexPath, std::string_view fragmentPath) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            LY_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return MakeRef<OpenGLShader>(vertexPath, fragmentPath);
    }

    LY_CORE_ASSERT(false, "Invalid RendererAPI!");
    return nullptr;
}

std::string Shader::ReadFile(std::string_view shaderFilePath) {
    // Open file in binary, and point the pointer to end of file.
    std::ifstream file(shaderFilePath.data(), std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error(std::format("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: {}",
                                             std::string(shaderFilePath)));
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
