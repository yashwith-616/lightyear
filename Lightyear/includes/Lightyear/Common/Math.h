#pragma once

#include "Lightyear/Common/Macros.h"
#include "Lightyear/Serialization/SerializationAdapters.h"

LY_DISABLE_WARNINGS_PUSH
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
LY_DISABLE_WARNINGS_POP

namespace ly {

template <>
struct SerializableAdapter<glm::vec2, void>;

template <>
struct SerializableAdapter<glm::vec3, void>;

template <>
struct SerializableAdapter<glm::vec4, void>;

template <>
struct SerializableAdapter<glm::uvec2, void>;

template <>
struct SerializableAdapter<glm::uvec3, void>;

template <>
struct SerializableAdapter<glm::uvec4, void>;

template <>
struct SerializableAdapter<glm::mat4, void>;

}  // namespace ly
