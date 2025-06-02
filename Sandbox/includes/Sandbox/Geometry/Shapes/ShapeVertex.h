#pragma once

#include <array>

// clang-format off

// ----- Cube Vertex Position -----
static constexpr std::array<std::array<float, 5>, 8> g_CubeVertices = { {
        // Positions           // UVs
        {{-0.5f, -0.5f, -0.5f, 0.0f, 0.0f}}, // 0 - Left Bottom Back
        {{ 0.5f, -0.5f, -0.5f, 1.0f, 0.0f}}, // 1 - Right Bottom Back
        {{ 0.5f,  0.5f, -0.5f, 1.0f, 1.0f}}, // 2 - Right Top Back
        {{-0.5f,  0.5f, -0.5f, 0.0f, 1.0f}}, // 3 - Left Top Back
        {{-0.5f, -0.5f,  0.5f, 0.0f, 0.0f}}, // 4 - Left Bottom Front
        {{ 0.5f, -0.5f,  0.5f, 1.0f, 0.0f}}, // 5 - Right Bottom Front
        {{ 0.5f,  0.5f,  0.5f, 1.0f, 1.0f}}, // 6 - Right Top Front
        {{-0.5f,  0.5f,  0.5f, 0.0f, 1.0f}}  // 7 - Left Top Front
    } };


// ----- Cube Index Position -----
constexpr uint32_t g_CubeIndices[] = {
    0, 1, 2, 2, 3, 0,  // Back face
    4, 5, 6, 6, 7, 4,  // Front face
    4, 7, 3, 3, 0, 4,  // Left face
    1, 5, 6, 6, 2, 1,  // Right face
    0, 1, 5, 5, 4, 0,  // Bottom face
    3, 2, 6, 6, 7, 3   // Top face
};

static std::array<std::array<float, 5>, 4> g_PlaneVertices{
    // { x,     y,     z,     u,     v }
    std::array<float, 5>{ -0.5f, -0.5f, 0.0f, 0.0f, 0.0f },  // Bottom-left
    std::array<float, 5>{ 0.5f, -0.5f, 0.0f, 1.0f, 0.0f },   // Bottom-right
    std::array<float, 5>{ 0.5f, 0.5f, 0.0f, 1.0f, 1.0f },    // Top-right
    std::array<float, 5>{ -0.5f, 0.5f, 0.0f, 0.0f, 1.0f }    // Top-left
};

static std::array<uint32_t, 6> g_PlaneIndices{ 0, 1, 2, 2, 3, 0 };

constexpr size_t g_CubeVertexCount = sizeof(g_CubeVertices);
constexpr size_t g_CubeIndexCount  = sizeof(g_CubeIndices);

constexpr size_t g_PlaneVertexCount = g_PlaneVertices.size() * sizeof(g_PlaneVertices[0]);
constexpr size_t g_PlaneIndexCount  = g_PlaneIndices.size() * sizeof(uint32_t);

// clang-format on