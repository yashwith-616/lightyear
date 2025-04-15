#pragma once

#include "Lightyear/LightyearAPI.h"
#include "glm/glm.hpp"

namespace ly {

	inline constexpr int MAX_BONE_INFLUENCE = 4;

	struct LIGHTYEAR_API Vertex {
		glm::vec3 position_;
		glm::vec3 normal_;
		glm::vec3 texCoord_;
		glm::vec3 tangent_;
		glm::vec3 bitangent_;

		int boneIDs_{ MAX_BONE_INFLUENCE };
		float weights_{ MAX_BONE_INFLUENCE };
	};
}