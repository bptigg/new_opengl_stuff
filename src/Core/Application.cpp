#include "Application.h"

#include "../utilities/log.h"

#include "../Rendering/renderer2d.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

Application* Application::s_instance = nullptr;

Application::Application(const Application_spec& spec)
	:m_spec(spec)
{
	if (s_instance != nullptr)
	{
		Log::crit("Application already exists", __FILE__, __LINE__);
	}

	if (!m_spec.Working_Directory.empty())
	{
		std::filesystem::current_path(m_spec.Working_Directory);
	}

	m_window = Window::Create(properties(m_spec.Name));
	m_window->Set_Event_Callback(BIND_EVENT_FN(Application::on_event));

	//Temp
	auto width = m_window->Get_Width();
	auto height = m_window->Get_Height();

	float aspect_ratio = width / height;
	m_camera = new Camera_Controller(aspect_ratio, false);
	//

	renderer2d::Init();

}

Application::~Application()
{
	renderer2d::Shutdown();
	glfwTerminate();
}

void Application::on_event(Events::Event& e)
{
	Events::Event_Dispatcher dispatcher(e);
	dispatcher.Dispatch<Events::Window_Close_Event>(BIND_EVENT_FN(Application::on_window_close));
	dispatcher.Dispatch<Events::Window_Resize_Event>(BIND_EVENT_FN(Application::on_window_resize));

	m_camera->On_Event(e);
}

void Application::Run()
{
	while (m_running)
	{
		float c_time = glfwGetTime();
		Timestep time = c_time - m_last_frame_time;
		m_last_frame_time = c_time;

		if (!m_minimized)
		{

		}
		m_window->On_Update();
	}
}

bool Application::on_window_close(Events::Window_Close_Event& e)
{
	m_running = false;
	return true;
}

bool Application::on_window_resize(Events::Window_Resize_Event& e)
{
	if (e.GetWidth() == 0 || e.GetHeight() == 0)
	{
		m_minimized = true;
		return false;
	}

	m_minimized = false;
	return false;
}

Application* create_application(Application_cmd_args args)
{
	Application_spec spec;
	spec.Name = "Testing";
	spec.cmd_args = args;

	return new Application(spec);
}
