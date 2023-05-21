#pragma once

#include "Event.h"

namespace Events
{
	class Key_Event : public Event
	{
	public:
		inline int Get_Key_Code() const { return m_Key_Code; }

		EVENT_CLASS_CATEGORY(Event_Catagory_Keyboard | Event_Catagory_Input);
	protected:
		Key_Event(int key_code)
			:m_Key_Code(key_code) {}

		int m_Key_Code;
	};


	class Key_Pressed_Event : public Key_Event
	{
	public:
		Key_Pressed_Event(int keycode, int repeat_count)
			:Key_Event(keycode), m_repeat_count(repeat_count) {}

		inline int Get_repeat_count() const { return m_repeat_count; }

		std::string To_String() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_Key_Code << " (" << m_repeat_count << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(Key_Pressed)
	private:
		int m_repeat_count;
	};

	class Key_Released_Event : public Key_Event
	{
	public:
		Key_Released_Event(int keycode)
			: Key_Event(keycode) {}

		std::string To_String() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_Key_Code;
			return ss.str();
		}

		EVENT_CLASS_TYPE(Key_Released)
	};

	class Key_Typed_Event : public Key_Event
	{
	public:
		Key_Typed_Event(int keycode)
			: Key_Event(keycode) {}

		std::string To_String() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_Key_Code;
			return ss.str();
		}

		EVENT_CLASS_TYPE(Key_Typed)
	};
}