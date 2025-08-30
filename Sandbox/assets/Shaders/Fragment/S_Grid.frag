#version 460 core

layout(std140, binding = 1) uniform Camera {
    mat4 u_ViewProjection;
    mat4 u_View;
    vec3 u_CameraPosition;// takes up 16 bytes due to std140 padding
    float u_ZoomLevel;// also padded to 16
};

layout(std140, binding = 2) uniform Scene {
    float u_Time;
};

layout(std140, binding = 3) uniform Material {
    vec4 u_BaseColor;
    vec4 u_Properties;
};

layout(std140, binding = 4) uniform Object {
    mat4 u_ModelMatrix;
    mat4 u_NormalMatrix;
};


in vec2 v_ScreenCoord;// Normalized screen coordinates [0, 1]

out vec4 FragColor;

void main() {
    FragColor = vec4(vec3(u_ModelMatrix[3]), u_Time);
}