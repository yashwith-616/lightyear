#pragma once

#include <array>
#include <span>

// clang-format off

// NOLINTNEXTLINE
static constexpr float g_CubeVertices[8][5] = {
    {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f},
    { 0.5f, -0.5f, -0.5f, 1.0f, 0.0f},
    { 0.5f,  0.5f, -0.5f, 1.0f, 1.0f},
    {-0.5f,  0.5f, -0.5f, 0.0f, 1.0f},
    {-0.5f, -0.5f,  0.5f, 0.0f, 0.0f},
    { 0.5f, -0.5f,  0.5f, 1.0f, 0.0f},
    { 0.5f,  0.5f,  0.5f, 1.0f, 1.0f},
    {-0.5f,  0.5f,  0.5f, 0.0f, 1.0f}
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
static constexpr float g_PlaneVertices[4][5] = {
    // { x,     y,     z,     u,     v }
    { -0.5f, -0.5f, 0.0f, 0.0f, 0.0f },  // Bottom-left
    {  0.5f, -0.5f, 0.0f, 1.0f, 0.0f },  // Bottom-right
    {  0.5f,  0.5f, 0.0f, 1.0f, 1.0f },  // Top-right
    { -0.5f,  0.5f, 0.0f, 0.0f, 1.0f }   // Top-left
};

// NOLINTNEXTLINE
static constexpr uint32_t g_PlaneIndices[] = {
    0, 1, 2, 2, 3, 0
};


constexpr auto g_CubeVerticesSpan = std::span{
    &g_CubeVertices[0][0],
    std::size(g_CubeVertices) * std::size(g_CubeVertices[0])
};
constexpr auto g_CubeIndicesSpan  = std::span{g_CubeIndices};

constexpr auto g_PlaneVerticesSpan = std::span{
    &g_PlaneVertices[0][0],
    std::size(g_PlaneVertices) * std::size(g_PlaneVertices[0])
};
constexpr auto g_PlaneIndicesSpan  = std::span{g_PlaneIndices};

// clang-format on