#pragma once

#include "Lightyear/Common/Math.h"
#include "Lightyear/pch/lypch.h"

namespace ly::scene {

struct MovementComponent {
    float Speed = 0;
    glm::vec3 Velocity;
    glm::vec3 Acceleration;
    glm::vec3 AngularVelocity;
    glm::vec3 AngularAcceleration;
};

}  // namespace ly::scene