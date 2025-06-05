#version 460 core

in vec2 v_ScreenCoord; // Normalized screen coordinates [0, 1]
out vec4 FragColor;

void main() {
    // Simple gradient or debug color
    vec3 color = vec3(v_ScreenCoord.x, v_ScreenCoord.y, 1.0 - v_ScreenCoord.x);
    FragColor = vec4(vec3(1.0), 1.0);
}