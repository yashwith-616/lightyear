#pragma once

#include "Lightyear/LightyearAPI.h"
#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"
#include <lypch.h>

namespace ly {

class LIGHTYEAR_API Mesh {
public:
    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;
    std::vector<Texture> textures_;
    unsigned int vao_;

    Mesh(std::vector<Vertex> vertexes, std::vector<unsigned int> indices, std::vector<Texture> textures);

    /**
     * @brief Draw the mesh with this shader
     * @param shader the shader
     */
    void draw(Shader& shader);

private:
    unsigned int vbo_;
    unsigned int ebo_;

    /**
     * @brief Initializes all the buffer objects/arrays
     */
    void setupMesh();
};
}
