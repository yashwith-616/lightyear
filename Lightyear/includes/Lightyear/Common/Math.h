#pragma once

#include "Lightyear/Common/Macros.h"

LY_DISABLE_WARNINGS_PUSH
#include <cereal/cereal.hpp>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

LY_DISABLE_WARNINGS_POP

namespace glm {

template <class Archive>
void serialize(Archive& ar, glm::vec3& v) {
    ar(v.x, v.y, v.z);
}

template <class Archive>
void serialize(Archive& ar, glm::vec4& v) {
    ar(v.x, v.y, v.z, v.w);
}

template <class Archive>
void serialize(Archive& ar, glm::mat4& m) {
    // Serialized by row
    for (int i = 0; i < 4; ++i) {
        ar(m[i][0], m[i][1], m[i][2], m[i][3]);
    }
}

}  // namespace glm
