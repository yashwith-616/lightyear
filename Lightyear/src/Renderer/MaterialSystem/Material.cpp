#include "Lightyear/Renderer/MaterialSystem/Material.h"

namespace ly::renderer {

ref<Material> Material::create(ref<Shader> shader) {
    auto material      = makeRef<Material>();
    material->m_shader = std::move(shader);
    return material;
}

}  // namespace ly::renderer