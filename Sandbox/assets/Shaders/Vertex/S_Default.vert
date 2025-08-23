#version 460 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;

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


out vec2 vTexCoord;

void main() {
    vTexCoord = aTexCoord;

    mat4 modelViewProjection = u_ViewProjection * u_ModelMatrix;
    gl_Position = modelViewProjection * vec4(aPosition, 1.0);
}
