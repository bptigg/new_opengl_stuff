#include "Window.h"

//#include "../Rendering/glDebug.h"
#include "../utilities/log.h"

Window::Window(const properties& prop)
{
	Init(prop);
}

Window::~Window()
{
	shutdown();
}

static void GLFWErrorCallback(int error, const char* description)
{
	std::stringstream ss;
	ss << "OPEN GL ERROR: " << error << " : " << description;
	Log::error(ss.str().c_str(), __FILE__, __LINE__);
}

void Window::On_Update()
{
	glfwPollEvents();
	glfwSwapBuffers(m_window);
}

unsigned int Window::Get_Width() const
{
	return m_data.width;
}

unsigned int Window::Get_Height() const
{
	return m_data.height;
}


void Window::Set_VSync(bool enabled)
{
	if (enabled)
	{
		glfwSwapInterval(1);
	}
	else
	{
		glfwSwapInterval(0);
	}

	m_data.VSync = enabled;
}

bool Window::Is_VSync() const
{
	return m_data.VSync;
}

Window* Window::Create(const properties& props)
{
	return new Window(props);
}

void Window::Init(const properties& props)
{
	m_data.name = props.m_name;
	m_data.width = props.m_width;
	m_data.height = props.m_height;

	if (!GLFW_initlized)
	{
		if (!glfwInit())
		{
			Log::crit("COULDN'T ESTABLISH VALID GLFW CONTEXT", __FILE__, __LINE__);
			glfwSetErrorCallback(GLFWErrorCallback);
		}
		GLFW_initlized = true;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow((int)props.m_width, (int)props.m_height, props.m_name.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(m_window);
	if (!m_window)
	{
		glfwTerminate();
		Log::crit("COULDN'T ESTABLISH VALID WINDOW CONTEXT", __FILE__, __LINE__);
	}

	Set_VSync(true);

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		Log::crit("GLEW NOT INITLIZED", __FILE__, __LINE__);
	};

	Log::info("OpenGL info:");
	Log::info((char*)glGetString(GL_VENDOR));
	Log::info((char*)glGetString(GL_RENDERER));
	Log::info((char*)glGetString(GL_VERSION));

	glfwSetWindowUserPointer(m_window, &m_data);

	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
		{
			window_data& data = *(window_data*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			Events::Window_Resize_Event event(width, height);
			data.Event_Call_back(event);
		});

	glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
		{
			window_data& data = *(window_data*)glfwGetWindowUserPointer(window);
			Events::Window_Close_Event event;
			data.Event_Call_back(event);
		});

	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			window_data& data = *(window_data*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				Events::Key_Pressed_Event event(key, 0);
				data.Event_Call_back(event);
				break;
			}
			case GLFW_RELEASE:
			{
				Events::Key_Released_Event event(key);
				data.Event_Call_back(event);
				break;
			}
			case GLFW_REPEAT:
			{
				Events::Key_Pressed_Event event(key, 1);
				data.Event_Call_back(event);
				break;
			}
			}
		});

	glfwSetCharCallback(m_window, [](GLFWwindow* window, uint32_t keycode)
		{
			window_data& data = *(window_data*)glfwGetWindowUserPointer(window);

			Events::Key_Typed_Event event(keycode);
			data.Event_Call_back(event);
		});

	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			window_data& data = *(window_data*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				double x_pos, y_pos;
				glfwGetCursorPos(window, &x_pos, &y_pos);
				Events::Mouse_Button_Pressed_Event event(button, (float)x_pos, (float)y_pos, (float)data.width, (float)data.height);
				data.Event_Call_back(event);
				break;
			}
			case GLFW_RELEASE:
			{
				Events::MouseButtonReleasedEvent event(button, (float)data.width, (float)data.height);
				data.Event_Call_back(event);
				break;
			}
			}
		});

	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			window_data& data = *(window_data*)glfwGetWindowUserPointer(window);

			Events::Mouse_Scrolled_Event event((float)xOffset, (float)yOffset);
			data.Event_Call_back(event);
		});

	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
		{
			window_data& data = *(window_data*)glfwGetWindowUserPointer(window);

			Events::Mouse_Moved_Event event((float)xPos, (float)yPos, (float)data.width, (float)data.height);
			data.Event_Call_back(event);
		});
}

void Window::shutdown()
{
	glfwDestroyWindow(m_window);
}
