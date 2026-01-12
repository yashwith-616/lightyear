#pragma once

#include <array>
#include <span>

// clang-format off

// NOLINTNEXTLINE
static constexpr float g_CubeVertices[] = {
    // x,     y,     z,     u,    v
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, // 0
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f, // 1
     0.5f,  0.5f, -0.5f,  1.0f,  1.0f, // 2
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f, // 3
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, // 4
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f, // 5
     0.5f,  0.5f,  0.5f,  1.0f,  1.0f, // 6
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f  // 7
};

// NOLINTNEXTLINE
static constexpr uint32_t g_CubeIndices[] = {
    0, 1, 2, 2, 3, 0,  // Back face
    4, 5, 6, 6, 7, 4,  // Front face
    4, 7, 3, 3, 0, 4,  // Left face
    1, 5, 6, 6, 2, 1,  // Right face
    0, 1, 5, 5, 4, 0,  // Bottom face
    3, 2, 6, 6, 7, 3   // Top face
};

// NOLINTNEXTLINE
static constexpr float g_PlaneVertices[] = {
    // { x,     y,     z,     u,     v }
     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f ,  // Bottom-left
      0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // Bottom-right
      0.5f,  0.5f, 0.0f, 1.0f, 1.0f,  // Top-right
     -0.5f,  0.5f, 0.0f, 0.0f, 1.0f   // Top-left
};

// NOLINTNEXTLINE
static constexpr uint32_t g_PlaneIndices[] = {
    0, 1, 2, 2, 3, 0
};


constexpr std::span k_cubeVerticesSpan{ g_CubeVertices };

constexpr auto k_cubeIndicesSpan  = std::span{g_CubeIndices};

constexpr std::span k_planeVerticesSpan { g_PlaneVertices };

constexpr auto k_planeIndicesSpan  = std::span{g_PlaneIndices};

// clang-format on