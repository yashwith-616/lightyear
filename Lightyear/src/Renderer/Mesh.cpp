#include "Lightyear/Renderer/Mesh.h"

#include <glad.h>

namespace ly {

Mesh::Mesh(std::vector<Vertex> vertexes,
           std::vector<unsigned int> indices,
           std::vector<Texture> textures) {
    this->vertices_ = vertexes;
    this->indices_  = indices;
    this->textures_ = textures;

    setupMesh();
}

void Mesh::draw(Shader& shader) {
    for (std::size_t i = 0; i < textures_.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(i));
        glBindTexture(GL_TEXTURE_2D, textures_[i].id_);

        std::string_view uniformName;
        switch (textures_[i].type) {
            case TextureType::DIFFUSE: uniformName = "u_diffuseTexture"; break;
            case TextureType::SPECULAR: uniformName = "u_specularTexture"; break;
            case TextureType::NORMAL: uniformName = "u_normalTexture"; break;
            case TextureType::HEIGHT: uniformName = "u_heightTexture"; break;
            default: continue;  // Skip unknown texture types.
        }

        // Set the uniform to the texture unit index
        shader.setUniform<int>(uniformName, static_cast<int>(i));
    }

    // Draw Mesh
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh() {
    // Generate the VAO, VBO, and EBO.
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    // Bind VAO (all subsequent buffer and attribute calls are stored in the VAO state)
    glBindVertexArray(vao_);

    // Bind and load vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(
        GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), vertices_.data(), GL_STATIC_DRAW);

    // Bind and load index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices_.size() * sizeof(unsigned int),
                 indices_.data(),
                 GL_STATIC_DRAW);

    // Set up vertex attribute pointers

    // Position attribute: attribute 0 (vec3), offset = 0.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));

    // Normal attribute: attribute 1 (vec3)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, normal_)));

    // Texture coordinates: attribute 2 (vec2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, texCoord_)));

    // Tangent attribute: attribute 3 (vec3)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, tangent_)));

    // Bitangent attribute: attribute 4 (vec3)
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, bitangent_)));

    // Bone IDs attribute: attribute 5 (ivec4)
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(
        5, 4, GL_INT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, boneIDs_)));

    // Bone Weights attribute: attribute 6 (vec4)
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6,
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, weights_)));

    // Unbind the VAO for now (optional, but good practice)
    glBindVertexArray(0);
}
}  // namespace ly
