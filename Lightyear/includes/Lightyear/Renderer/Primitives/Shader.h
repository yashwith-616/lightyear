#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Renderer/Primitives/RenderTypes.h"

namespace ly::renderer
{

class LIGHTYEAR_API Shader
{
public:
    virtual ~Shader() = default;

    static Ref<Shader>
        Create(const std::string& name, const std::unordered_map<ShaderType, std::filesystem::path>& shaderFiles);
    static Ref<Shader> Create(const std::string& name, const std::unordered_map<ShaderType, std::string>& shaderSrcs);

    virtual void Use() const = 0;
    virtual void UnBind() const = 0;

    [[nodiscard]] virtual const std::string& GetName() const { return m_Name; }

protected:
    std::string m_Name{};

    /**
     * @brief Read file from path.
     * @param shaderFilePath the shader file path
     * @return string with the file data
     */
    static std::string ReadFile(const std::filesystem::path& shaderFilePath);
};

} // namespace ly::renderer
