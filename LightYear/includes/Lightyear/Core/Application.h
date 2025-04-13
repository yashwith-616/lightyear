#pragma once

#include "Lightyear/LightyearAPI.h"

namespace ly {
	class LIGHTYEAR_API Application
	{
	public:
		Application();
		~Application();
		virtual void Run();
	};
}