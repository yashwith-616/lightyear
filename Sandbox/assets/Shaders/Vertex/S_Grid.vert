#version 460 core

layout(location = 0) in vec2 aPosition; // Vertex in NDC: [-1, -1] to [1, 1]

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

out vec2 v_ScreenCoord; // [0, 1] normalized screen coordinates

void main() {
    v_ScreenCoord = aPosition * 0.5 + 0.5; // NDC [-1, 1] → Screen [0, 1]
    gl_Position = vec4(aPosition, 0.0, 1.0);
}