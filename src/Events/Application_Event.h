#pragma once

#include "Event.h"

namespace Events {

	class Window_Resize_Event : public Event
	{
	public:
		Window_Resize_Event(uint32_t width, uint32_t height)
			: m_Width(width), m_Height(height) {}

		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }

		std::string To_String() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(Window_Resize)
		EVENT_CLASS_CATEGORY(Event_Catagory_Application)
	private:
		uint32_t m_Width, m_Height;
	};

	class Window_Close_Event : public Event
	{
	public:
		Window_Close_Event() {}

		EVENT_CLASS_TYPE(Window_Close)
		EVENT_CLASS_CATEGORY(Event_Catagory_Application)
	};

	class App_Tick_Event : public Event
	{
	public:
		App_Tick_Event() {}

		EVENT_CLASS_TYPE(App_Tick)
		EVENT_CLASS_CATEGORY(Event_Catagory_Application)
	};

	class App_Update_Event : public Event
	{
	public:
		App_Update_Event() {}

		EVENT_CLASS_TYPE(App_Update)
		EVENT_CLASS_CATEGORY(Event_Catagory_Application)
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(App_Render)
		EVENT_CLASS_CATEGORY(Event_Catagory_Application)
	};

	class AppExportEvent : public Event
	{
	public:
		AppExportEvent() {}

		EVENT_CLASS_TYPE(App_Export)
		EVENT_CLASS_CATEGORY(Event_Catagory_Application)
	};
}