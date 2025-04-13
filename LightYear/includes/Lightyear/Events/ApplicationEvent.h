#pragma once

#include "Event.h"

namespace ly {

	class LIGHTYEAR_API WindowResizeEvent : public Event {
	public:
		// height and width after resize
	};

	class LIGHTYEAR_API WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EC_APPLICATION)
	};


	// TODO: The below may not be required
	class LIGHTYEAR_API AppTickEvent : public Event {

	};

	class LIGHTYEAR_API AppUpdatedEvent : public Event {

	};

	class LIGHTYEAR_API AppRenderEvent : public Event {

	};
}