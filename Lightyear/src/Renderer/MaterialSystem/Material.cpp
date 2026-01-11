#include "Lightyear/Renderer/MaterialSystem/Material.h"

namespace ly::renderer
{

Ref<Material> Material::Create(Ref<Shader> shader)
{
    auto material = MakeRef<Material>();
    material->m_Shader = std::move(shader);
    return material;
}

} // namespace ly::renderer
