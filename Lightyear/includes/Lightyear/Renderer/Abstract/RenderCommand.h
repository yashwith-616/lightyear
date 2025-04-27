#pragma once

#include <lypch.h>
#include <glm/glm.hpp>
#include "Lightyear/LightyearCore.h"

namespace ly::renderer {

class RendererAPI;
class VertexArray;

class LIGHTYEAR_API RenderCommand {
public:
    inline static void SetClearColor(const glm::vec4& color);

    inline static void Clear();

    inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray);
    inline static void DrawIndexed(const VertexArray& vertexArray);

private:
    static RendererAPI* s_RendererAPI;
};

}  // namespace ly::renderer