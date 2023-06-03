#include "Application.h"

#include "../utilities/log.h"

#include "../Rendering/renderer2d.h"

#include "../Rendering/Framebuffer.h"

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
	float width = m_window->Get_Width();
	float height = m_window->Get_Height();

	float aspect_ratio = width / height;
	m_camera = new Camera_Controller(width, height, false);
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

	//temp code
	float old_1 = 0, old_2 = 0, old_3 = 0;
	Framebufferspec fbspec;
	fbspec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
	fbspec.width = 1280;
	fbspec.height = 720;
	auto framebuffer = Framebuffer::Create(fbspec);

	std::shared_ptr<Texture_Data> screen = std::make_shared<Texture_Data>(Texture_Data());
	screen->texture_id = framebuffer->get_color_attachment_renderer_id();
	screen->size = { framebuffer->GetSpec().width, framebuffer->GetSpec().height };
	screen->bound = false;
	screen->slot = 0;
	screen->alive = true;

	std::string screen_name = "m_screen";
	renderer2d::get_texture_library()->Add(screen_name, screen);
	renderer2d::get_subtexture_library()->create("s_screen", screen_name, *screen, { 0,0 }, { screen->size.x, screen->size.y }, { 1,1 });

	QUADrender_param pic;
	pic.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	pic.layer = 1;
	pic.size = { 1280.0f, 720.0f };
	pic.rotation = 0;
	pic.Texture = "s_screen";


	while (m_running)
	{
		renderer2d::clear();

		float c_time = glfwGetTime();
		Timestep time = c_time - m_last_frame_time;
		m_last_frame_time = c_time;

		renderer2d::Begin_Scene(m_camera->get_camera());

		if (!m_minimized)
		{
			m_camera->On_Update(time);
			CIRCLErender_param test_2;
			test_2.color = { 0.4f, 0.3f, 0.7f, 1.0f };
			test_2.thickness = 0.5f;
			test_2.fade = 0.005f;
			test_2.layer = 1;

			glm::vec3 translation = { 720.0f, 480.0f, 1.0f };
			glm::vec3 scale = { 30.0f, 30.0f, 1.0f };

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation) * glm::scale(glm::mat4(1.0f), scale);
			test_2.transform = transform;

			renderer2d::draw_circle(test_2);

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

			LINErender_param line;
			line.color = { 1.0f, 0.0f, 0.5f, 1.0f };
			line.layer = 2;
			line.p0 = { 200.0f, 200.0f, 1.0f };
			line.p1 = { 800.0f, 400.0f, 1.0f };
			renderer2d::draw_line(line);

			Textrender_param text;
			text.centered = true;
			text.color = { 0.5f, 0.6f, 0.2f, 1.0f };
			text.layer = 3;
			text.scale = 100.0f;
			//text.text = "Hello world";
			text.text = "hello";

			glm::vec2 pos = { 640.0f, 500.0f };
			renderer2d::draw_text(text, pos);


			framebuffer->Bind();
			framebuffer->clear_attachment(0, -1);
			framebuffer->clear_attachment(1, -1);
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			renderer2d::End_Scene();

			framebuffer->Unbind();

			renderer2d::get_texture_library()->get("m_screen")->texture_id = framebuffer->get_color_attachment_renderer_id();

			framebuffer->Unbind();


			glm::mat4 mvp = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
			renderer2d::get_shader_library()->get("Quad")->set_uniform_mat_4f("u_view_proj", mvp);

			renderer2d::draw_quad({ 640.0f, 360.0f }, pic);
			renderer2d::draw();

		}
		m_window->On_Update();
	}
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
