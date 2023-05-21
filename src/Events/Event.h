#pragma once

#include <string>
#include <functional>
#include <sstream>

#define BIT(X) (1 << X)

namespace Events {

	enum class Event_Type
	{
		None = 0,
		Window_Close, Window_Resize, Window_Focus, Window_Lost_Focus, Window_Moved,
		App_Tick, App_Update, App_Render, App_Export,
		Key_Pressed, Key_Released,Key_Typed,
		Mouse_Button_Pressed, Mouse_Button_Released, Mouse_Moved, Mouse_Scrolled,
		
	};

	enum Event_Catagory
	{
		None = 0,
		Event_Catagory_Application		= BIT(0),
		Event_Catagory_Input			= BIT(1),
		Event_Catagory_Keyboard			= BIT(2),
		Event_Catagory_Mouse			= BIT(3),
		Event_Catagory_Mouse_Button		= BIT(4),

	};

	class Event
	{
		friend class Event_Dispatcher;
	private:
		bool m_Handled = false;
	public:
		virtual Event_Type Get_Event_Type() const = 0;
		virtual const char* Get_Name() const = 0;
		virtual int Get_Catagory_Flag() const = 0;
		virtual std::string To_String() const { return Get_Name(); }

		inline bool Is_In_Catagory(Event_Catagory cat)
		{
			return Get_Catagory_Flag() & cat;
		}
	public:
		bool& Handled = m_Handled;
	};

#define EVENT_CLASS_TYPE(type) static Event_Type GetStaticType() { return Event_Type::##type; }\
																virtual Event_Type Get_Event_Type() const override { return GetStaticType(); }\
																virtual const char* Get_Name() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int Get_Catagory_Flag() const override { return category; }

	class Event_Dispatcher
	{
	public:
		Event_Dispatcher(Event& event)
			:m_Event(event)
		{
		}

		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_Event.Get_Event_Type() == T::GetStaticType())
			{
				m_Event.Handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.To_String();
	}


}