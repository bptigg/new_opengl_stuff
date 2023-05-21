#pragma once

#include "../Events/Application_Event.h"
#include "../Events/Key_Event.h"
#include "../Events/Mouse_Event.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct properties
{
	std::string m_name;
	unsigned int m_height;
	unsigned int m_width;

	properties(const std::string& name = "Defualt Window", unsigned int width = 1280, unsigned int height = 720)
		:m_name(name), m_width(width), m_height(height)
	{

	}
};

class Window
{
public:
	using Event_Call_back_fn = std::function<void(Events::Event&)>;
	Window(const properties& prop);
	virtual ~Window();

	void On_Update();

	unsigned int Get_Width() const;
	unsigned int Get_Height() const;

	// Window attributes
	inline void Set_Event_Callback(const Event_Call_back_fn& callback) { m_data.Event_Call_back = callback; }
	void Set_VSync(bool enabled);
	bool Is_VSync() const;

	virtual void* Get_Native_Window() const { return m_window; }
	GLFWwindow* Get_Native_Window() { return m_window; }

	static Window* Create(const properties& props = properties());
private:
	virtual void Init(const properties& props);
	virtual void shutdown();
private:
	GLFWwindow* m_window;
	bool GLFW_initlized;

	struct window_data
	{
		std::string name;
		unsigned int width, height;
		bool VSync;

		Event_Call_back_fn Event_Call_back;
	};

	window_data m_data;
};