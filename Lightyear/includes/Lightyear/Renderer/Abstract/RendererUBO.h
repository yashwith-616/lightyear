#pragma once

#include "Lightyear/Renderer/Primitives/UniformBuffer.h"

namespace ly::renderer {

struct alignas(16) UboCamera {
    glm::mat4 uViewProjection;  // 64 bytes
    glm::mat4 uView;            // 64 bytes
    glm::vec3 uCameraPosition;  // 12 bytes
    float padCameraPos{};       // 4 bytes - Alignment
    float uZoomLevel;           // 4 bytes
    glm::vec3 padZoom{};        // 12 bytes - Alignment
};

struct alignas(16) UboScene {
    float uTime;  // padded to 16
    glm::vec3 padTime;
};

struct alignas(16) UboMaterial {
    glm::vec4 uBaseColor;   // 16 bytes
    glm::vec4 uProperties;  // 16 bytes
};

struct alignas(16) UboObject {
    glm::mat4 uModelMatrix;   // 64 bytes
    glm::mat4 uNormalMatrix;  // 64 bytes
};

struct LIGHTYEAR_API GlobalUniforms {
    ref<UniformBuffer> cameraUbo;
    ref<UniformBuffer> materialUbo;
    ref<UniformBuffer> objectUbo;
    ref<UniformBuffer> sceneUbo;

    void init() {
        cameraUbo   = UniformBuffer::create("Camera", sizeof(UboCamera), 1);      // binding = 0
        sceneUbo    = UniformBuffer::create("Scene", sizeof(UboScene), 2);        // binding = 1
        materialUbo = UniformBuffer::create("Material", sizeof(UboMaterial), 3);  // binding = 2
        objectUbo   = UniformBuffer::create("Object", sizeof(UboObject), 4);      // binding = 3
    }

    void bind() const {
        cameraUbo->bind();
        materialUbo->bind();
        objectUbo->bind();
        sceneUbo->bind();
    }

    void uploadCamera(UboCamera const& data) const { cameraUbo->setData(&data, sizeof(UboCamera), 0); }

    void uploadMaterial(UboMaterial const& data) const { materialUbo->setData(&data, sizeof(UboMaterial), 0); }

    void uploadObject(UboObject const& data) const { objectUbo->setData(&data, sizeof(UboObject), 0); }

    void uploadScene(UboScene const& data) const { sceneUbo->setData(&data, sizeof(UboScene), 0); }
};

}  // namespace ly::renderer