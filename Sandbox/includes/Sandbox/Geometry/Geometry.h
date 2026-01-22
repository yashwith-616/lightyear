#pragma once

#include "Lightyear.h"

class Geometry
{
public:
    ly::ref<ly::renderer::VertexArray> cubeVertexArray;
    ly::ref<ly::renderer::VertexArray> sphereVertexArray;
    ly::ref<ly::renderer::VertexArray> cylinderVertexArray;
    ly::ref<ly::renderer::VertexArray> teapotVertexArray;
    ly::ref<ly::renderer::VertexArray> planeVertexArray;

    Geometry();

    static ly::ref<ly::renderer::VertexArray> getPlane();

    static ly::ref<ly::renderer::VertexArray> getCube();

    static ly::ref<ly::renderer::VertexArray> getSphere();

    static ly::ref<ly::renderer::VertexArray> getCylinder();

    /**
     * @brief This is an OpenGL only function
     */
    static ly::ref<ly::renderer::VertexArray> getTeapot();

    static Geometry* geometry;
};
