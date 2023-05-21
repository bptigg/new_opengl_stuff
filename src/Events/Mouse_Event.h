#pragma once

#include "Event.h"

namespace Events {

	class Mouse_Moved_Event : public Event
	{
	public:
		Mouse_Moved_Event(float x, float y, float width, float height)
			: m_Mouse_X(x), m_Mouse_Y(y), m_width(width), m_height(height) {}

		inline float GetX() const { return m_Mouse_X; }
		inline float GetY() const { return m_Mouse_Y; }

		inline float get_width() const { return m_width; }
		inline float get_height() const { return m_height; }

		std::string To_String() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_Mouse_X << ", " << m_Mouse_Y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(Mouse_Moved)
		EVENT_CLASS_CATEGORY(Event_Catagory_Mouse | Event_Catagory_Input)
	private:
		float m_Mouse_X, m_Mouse_Y;
		float m_width, m_height;
	};

	class Mouse_Scrolled_Event : public Event
	{
	public:
		Mouse_Scrolled_Event(float x_Offset, float y_Offset)
			: m_X_Offset(x_Offset), m_Y_Offset(y_Offset) {}

		inline float GetXOffset() const { return m_X_Offset; }
		inline float GetYOffset() const { return m_Y_Offset; }

		std::string To_String() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(Mouse_Scrolled)
		EVENT_CLASS_CATEGORY(Event_Catagory_Mouse | Event_Catagory_Input)
	private:
		float m_X_Offset, m_Y_Offset;
	};

	class Mouse_Button_Event : public Event
	{
	public:
		inline int Get_Mouse_Button() const { return m_Button; }

		inline float get_width() const { return m_width; }
		inline float get_height() const { return m_height; }

		EVENT_CLASS_CATEGORY(Event_Catagory_Mouse | Event_Catagory_Input)

	protected:
		Mouse_Button_Event(int button, float width, float height)
			: m_Button(button), m_width(width), m_height(height) {}

		int m_Button;
		float m_width, m_height;
	};

	class Mouse_Button_Pressed_Event : public Mouse_Button_Event
	{
	public:
		Mouse_Button_Pressed_Event(int button, float x, float y, float width, float height)
			: Mouse_Button_Event(button, width, height), m_Mouse_X(x), m_Mouse_Y(y) {}

		inline float get_x() const { return m_Mouse_X; }
		inline float get_y() const { return m_Mouse_Y; }

		std::string To_String() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(Mouse_Button_Pressed)
	private:
		float m_Mouse_X, m_Mouse_Y;
	
	};

	class MouseButtonReleasedEvent : public Mouse_Button_Event
	{
	public:
		MouseButtonReleasedEvent(int button, float x, float y)
			: Mouse_Button_Event(button, x, y) {}

		std::string To_String() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(Mouse_Button_Released)
	};

}