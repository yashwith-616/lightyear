#pragma once

#include "Lightyear/Renderer/Primitives/UniformBuffer.h"

namespace ly::renderer {

struct alignas(16) UBO_Camera {
    glm::mat4 u_ViewProjection;  // 64 bytes
    glm::mat4 u_View;            // 64 bytes
    glm::vec3 u_CameraPosition;  // 12 bytes
    float _padCameraPos{};       // 4 bytes - Alignment
    float u_ZoomLevel;           // 4 bytes
    glm::vec3 _padZoom{};        // 12 bytes - Alignment
};

struct alignas(16) UBO_Scene {
    float u_Time;  // padded to 16
    glm::vec3 _padTime;
};

struct alignas(16) UBO_Material {
    glm::vec4 u_BaseColor;   // 16 bytes
    glm::vec4 u_Properties;  // 16 bytes
};

struct alignas(16) UBO_Object {
    glm::mat4 u_ModelMatrix;   // 64 bytes
    glm::mat4 u_NormalMatrix;  // 64 bytes
};

struct LIGHTYEAR_API GlobalUniforms {
    Ref<UniformBuffer> CameraUBO;
    Ref<UniformBuffer> MaterialUBO;
    Ref<UniformBuffer> ObjectUBO;
    Ref<UniformBuffer> SceneUBO;

    void Init() {
        CameraUBO   = UniformBuffer::Create("Camera", sizeof(UBO_Camera), 1);      // binding = 0
        SceneUBO    = UniformBuffer::Create("Scene", sizeof(UBO_Scene), 2);        // binding = 1
        MaterialUBO = UniformBuffer::Create("Material", sizeof(UBO_Material), 3);  // binding = 2
        ObjectUBO   = UniformBuffer::Create("Object", sizeof(UBO_Object), 4);      // binding = 3
    }

    void Bind() const {
        CameraUBO->Bind();
        MaterialUBO->Bind();
        ObjectUBO->Bind();
        SceneUBO->Bind();
    }

    void UploadCamera(const UBO_Camera& data) const { CameraUBO->SetData(&data, sizeof(UBO_Camera), 0); }

    void UploadMaterial(const UBO_Material& data) const { MaterialUBO->SetData(&data, sizeof(UBO_Material), 0); }

    void UploadObject(const UBO_Object& data) const { ObjectUBO->SetData(&data, sizeof(UBO_Object), 0); }

    void UploadScene(const UBO_Scene& data) const { SceneUBO->SetData(&data, sizeof(UBO_Scene), 0); }
};

}  // namespace ly::renderer