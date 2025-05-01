#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Renderer/Primitives/RenderTypes.h"

namespace ly::renderer {

class Shader;

class LIGHTYEAR_API Material {
public:
    virtual ~Material() = default;

    static Ref<Material> Create(Ref<Shader> shader);

private:
    Ref<Shader> m_Shader;
};

}  // namespace ly::renderer