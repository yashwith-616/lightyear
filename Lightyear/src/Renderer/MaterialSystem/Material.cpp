#include "Lightyear/Renderer/MaterialSystem/Material.h"

namespace ly::renderer {

Ref<Material> Material::Create(Ref<Shader> shader) {
    Material* material = new Material();
    material->m_Shader = shader;
    return Ref<Material>(material);
}

}  // namespace ly::renderer