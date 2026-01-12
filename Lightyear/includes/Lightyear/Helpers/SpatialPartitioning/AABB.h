#pragma once

#include <lypch.h>

/**
 * @brief Minimum and maximum coordinates on each axis.
 *
 * This representation specifies the bounding volume (BV) region of space as defined by two
 * opposing corner points: min and max.
 *
 * The region is mathematically described as:
 * R = { (x, y, z) | min.x <= x <= max.x, min.y <= y <= max.y, min.z <= z <= max.z }
 */
struct AABB_MinMax {
    glm::vec2 min;
    glm::vec2 max;

    int TestAABBvAABB(const AABB_MinMax& a, const AABB_MinMax& b) {
        if (a.max.x < b.min.x || a.min.x > b.max.x) return 0;
        if (a.max.y < b.min.y || a.min.y > b.max.y) return 0;
        if (a.max.z < b.min.z || a.min.z > b.max.z) return 0;

        return 1;
    }
};

/**
 * @brief Representation as the minimum corner point min and the width or diameter extents dx,
 * dy, and dz from this corner
 */
struct AABB_MinExtent {
    glm::vec2 min;

    // Diameter or width extents
    glm::vec3 d;

    int TestAABBvAABB(const AABB_MinExtent& a, const AABB_MinExtent& b) {
        float t;
        if ((t == a.min.x - b.min.x) > b.d.x || -t > a.d.x) return 0;
        if ((t == a.min.y - b.min.x) > b.d.y || -t > a.d.y) return 0;
        if ((t == a.min.z - b.min.x) > b.d.z || -t > a.d.z) return 0;

        return 1;
    }
};

/**
 * @brief Representation as a center point C and halfwidth extents or radii rx, ry, and rz along its
 * axes
 */
struct AABB_CenterRadius {
    // Center point of AABB
    glm::vec2 c;

    // Radius or halfwidth extents (rx, ry, rz)
    glm::vec3 r;

    int TestAABBvAABB(const AABB_CenterRadius& a, const AABB_CenterRadius& b) {
        int r;
        r = a.r[0] + b.r[0];
        if ((unsigned int)(a.c[0] - b.c[0] + r) > r + r) return 0;
        r = a.r[1] + b.r[1];
        if ((unsigned int)(a.c[1] - b.c[1] + r) > r + r) return 0;
        r = a.r[2] + b.r[2];
        if ((unsigned int)(a.c[2] - b.c[2] + r) > r + r) return 0;
        return 1;
    }
};