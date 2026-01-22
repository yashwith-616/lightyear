#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Renderer/Primitives/RenderTypes.h"

namespace ly::renderer
{

class LIGHTYEAR_API Shader
{
public:
    virtual ~Shader() = default;

    static ref<Shader>
        create(std::string const& name, std::unordered_map<ShaderType, std::filesystem::path> const& shaderFiles);
    static ref<Shader> create(std::string const& name, std::unordered_map<ShaderType, std::string> const& shaderSrcs);

    virtual void use() const = 0;
    virtual void unBind() const = 0;

    [[nodiscard]] virtual std::string const& getName() const { return m_name; }

protected:
    std::string m_name{};

    /**
     * @brief Read file from path.
     * @param shaderFilePath the shader file path
     * @return string with the file data
     */
    static std::string readFile(std::filesystem::path const& shaderFilePath);
};

} // namespace ly::renderer
