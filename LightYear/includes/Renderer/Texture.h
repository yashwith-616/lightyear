#pragma once

#include <lypch.h>

namespace Lightyear::renderer {
	
	enum class TextureType : uint8_t {
		DIFFUSE,
		SPECULAR,
		NORMAL,
		HEIGHT
	};
	
	class Texture {
	public:
		unsigned int id_;
		TextureType type;
	};
}