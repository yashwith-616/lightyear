#pragma once

#include <lypch.h>
#include <glm/glm.hpp>

namespace ly::renderer {

struct alignas(16) UBO_Camera {
    glm::mat4 ViewProjection{};  // 64 bytes
    glm::mat4 View{};            // 64 bytes
    glm::vec3 CameraPosition{};  // 12 bytes
    float _pad1;                 // 4 bytes (to align to 16 bytes)
    glm::vec3 CameraRotation{};  // 12 bytes
    float ZoomLevel;             // 4 bytes (to align to 16 bytes)
};

struct UBO_Scene {
    float Time{ 1.f };  // 4
};

struct alignas(16) UBO_Material {
    glm::vec4 BaseColor{};   // Could be used for tint (16 bytes)
    glm::vec4 Properties{};  // 4 (to align to 16)
};

struct alignas(16) UBO_Object {
    glm::mat4 ModelMatrix{};
    glm::mat4 NormalMatrix{};  // Optional: for correct lighting
};

struct LIGHTYEAR_API GlobalUniforms {
    Ref<UniformBuffer> CameraUBO;
    Ref<UniformBuffer> MaterialUBO;
    Ref<UniformBuffer> ObjectUBO;
    Ref<UniformBuffer> SceneUBO;

    void Init() {
        CameraUBO   = UniformBuffer::Create("Camera", sizeof(UBO_Camera), 0);
        SceneUBO    = UniformBuffer::Create("Scene", sizeof(UBO_Scene), 1);
        MaterialUBO = UniformBuffer::Create("Material", sizeof(UBO_Material), 2);
        ObjectUBO   = UniformBuffer::Create("Object", sizeof(UBO_Object), 3);
    }

    void Bind() {
        CameraUBO->Bind();
        MaterialUBO->Bind();
        ObjectUBO->Bind();
        SceneUBO->Bind();
    }

    void UploadCamera(const UBO_Camera& data) { CameraUBO->SetData(&data, sizeof(UBO_Camera), 0); }

    void UploadMaterial(const UBO_Material& data) {
        MaterialUBO->SetData(&data, sizeof(UBO_Material));
    }

    void UploadObject(const UBO_Object& data) { ObjectUBO->SetData(&data, sizeof(UBO_Object), 0); }

    void UploadScene(const UBO_Scene& data) { SceneUBO->SetData(&data, sizeof(UBO_Scene), 0); }
};

}  // namespace ly::renderer