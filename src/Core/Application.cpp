#include "Application.h"

#include "../utilities/log.h"

#include "../Rendering/renderer2d.h"

#define _USE_MATH_DEFINES
#include <math.h>


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
	renderer2d::enable_blending();

	float old_1 = 0, old_2 = 0, old_3 = 0;

	while (m_running)
	{
		renderer2d::clear();

		float c_time = glfwGetTime();
		Timestep time = c_time - m_last_frame_time;
		m_last_frame_time = c_time;

		if (!m_minimized)
		{
			QUADrender_param test;
			test.color = { 0.5f, 0.2f, 0.3f, 1.0f };
			test.layer = 1;
			test.size = {60.0f, 60.0f };
			test.rotation = old_1 + 0.4 * time;
			old_1 = test.rotation;
			test.Texture = "";

			renderer2d::draw_quad({ 640.0f , 360.0f }, test);

			test.color = { 0.6f, 0.8f, 0.2f, 0.8f };
			test.layer = 2;
			test.size = { 100.0f, 53.0f };
			test.rotation = old_2 + 0.2 * time;
			old_2 = test.rotation;
			test.Texture = "";

			renderer2d::draw_quad({ 400.0f , 100.0f }, test);

			test.color = { 0.0f, 0.0f, 1.0f, 0.2f };
			test.layer = 3;
			test.size = { 30.0f, 30.0f };
			test.rotation = old_3 + 1*time;
			old_3 = test.rotation;
			test.Texture = "";

			renderer2d::draw_quad({ 640.0f , 360.0f }, test);
			renderer2d::draw();
		}
		m_window->On_Update();
	}
	//renderer2d::Shutdown();
	return;
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
