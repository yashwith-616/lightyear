#version 460 core

layout(location = 0) in vec3 aPosition; // Fullscreen quad: [-1, -1] to [1, 1]
layout(location = 0) in vec2 aTexCoord; // Fullscreen quad: [-1, -1] to [1, 1]

layout(std140, binding = 0) uniform Camera {
    mat4 u_ViewProjection;
    mat4 u_View;
    vec3 u_CameraPosition;
    vec3 u_CameraRotation;
    float u_ZoomLevel;
};

layout(std140, binding = 1) uniform Scene {
    float u_Time;
};

layout(std140, binding = 2) uniform Material {
    vec4 u_BaseColor;
    vec4 u_Properties;
};

layout(std140, binding = 3) uniform Object {
    mat4 u_ModelMatrix;
    mat4 u_NormalMatrix;
};

void main() {
    mat4 modelViewProjection = u_ViewProjection * u_ModelMatrix;
    gl_Position = modelViewProjection * vec4(aPosition, 1.0);
}