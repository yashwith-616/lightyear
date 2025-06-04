#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

layout(std140, binding = 0) uniform Camera {
    mat4 u_ViewProjection;
    mat4 u_View;
    vec3 u_CameraPosition;
    vec3 u_CameraRotation;
};

layout(std140, binding = 1) uniform Scene {
    float u_Time;
};

layout(std140, binding = 2) uniform Material {
    vec4 u_BaseColor;
    float u_Roughness;
    float u_Metallic;
    float u_AO;
};

layout(std140, binding = 3) uniform Object {
    mat4 u_ModelMatrix;
    mat4 u_NormalMatrix;
};

out vec2 v_ScreenCoord;

void main() {
    v_ScreenCoord = (a_Position.xy + 1.0) * 0.5; // Normalized to [0,1]
    gl_Position = vec4(a_Position, 1.0);
}