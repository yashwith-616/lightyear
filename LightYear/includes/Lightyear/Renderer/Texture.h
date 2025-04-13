#pragma once

#include "Lightyear/LightyearAPI.h"
#include <lypch.h>

namespace Lightyear::renderer {
	
	enum class TextureType : uint8_t {
		DIFFUSE,
		SPECULAR,
		NORMAL,
		HEIGHT
	};
	
	class LIGHTYEAR_API Texture {
	public:
		unsigned int id_;
		TextureType type;
	};
}