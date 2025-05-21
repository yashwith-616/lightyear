#version 460 core

in vec3 v_WorldPos;

uniform sampler2D u_Color;
uniform float u_GridSize = 0.1;
uniform vec3 u_LineColor = vec3(1.0);
uniform vec3 u_BackgroundColor = vec3(0.3);

out vec4 FragColor;

void main() {
    vec2 pos = v_WorldPos.xy;

    // Use modulus to detect grid lines
    float lineX = abs(fract(pos.x / u_GridSize) - 0.5);
    float lineY = abs(fract(pos.y / u_GridSize) - 0.5);

    float lineWidth = 0.02; // Fraction of gridSize (tweak for sharpness)

    float isLine = step(lineX, lineWidth) + step(lineY, lineWidth);
    isLine = clamp(isLine, 0.0, 1.0);

    vec3 color = mix(u_BackgroundColor, u_LineColor, isLine);
    FragColor = vec4(color, 1.0);
}
