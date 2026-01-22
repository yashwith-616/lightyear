#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly::renderer
{

class Shader;

class LIGHTYEAR_API Material
{
public:
    virtual ~Material() = default;

    static ref<Material> create(ref<Shader> shader);

private:
    ref<Shader> m_shader;
};

} // namespace ly::renderer
