#version 460 core

in vec2 v_ScreenCoord;

uniform sampler2D u_Color;
uniform float u_GridSize = 0.1;
uniform vec3 u_LineColor = vec3(1.0);
uniform vec3 u_BackgroundColor = vec3(0.3);

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

out vec4 FragColor;

void main() {
    float brightness = sin(u_Time * 2.0 * 3.14159); // [0,1] range
    vec3 color = vec3(brightness);             // grayscale brightness
    FragColor = vec4(u_Time, 0, 0, 1.0);
}
