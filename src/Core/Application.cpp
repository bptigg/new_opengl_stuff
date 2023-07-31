#include "Application.h"

#include "../utilities/log.h"

#include "../Rendering/renderer2d.h"

#include "../Rendering/Framebuffer.h"

#include "../utilities/utility.h"

#include "../Rendering/Post_Processing.h"
#include "../Rendering/Light.h";

#include "../Core/Geometry.h"

#define _USE_MATH_DEFINES
#include <math.h>


#include <stdlib.h> //just for testing code
#include <time.h>


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
	s_instance = this;

}

Application::~Application()
{
	renderer2d::Shutdown();
	glfwTerminate();
	Log::info("APPLICATION TERMINATED");
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

	GLint max_samples = 0;
	glGetIntegerv(GL_MAX_INTEGER_SAMPLES, &max_samples);
	std::cout << max_samples << std::endl;

	//temp code
	Framebufferspec fbspec;
	fbspec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RGBA8_SNORM};
	fbspec.width = 1280;
	fbspec.height = 720;
	fbspec.samples = 1;
	auto framebuffer = Framebuffer::Create(fbspec);

	Framebufferspec fbspec2;
	fbspec2.Attachments = { FramebufferTextureFormat::RGBA8};
	fbspec2.width = 1280;
	fbspec2.height = 720;
	auto framebuffer2 = Framebuffer::Create(fbspec2);

	Framebufferspec fbspec3;
	fbspec3.Attachments = { FramebufferTextureFormat::RGBA8_SNORM};
	fbspec3.width = 1280;
	fbspec3.height = 720;
	auto framebuffer3 = Framebuffer::Create(fbspec3);

	std::shared_ptr<Texture_Data> screen = std::make_shared<Texture_Data>(Texture_Data());
	screen->texture_id = framebuffer->get_color_attachment_renderer_id(0);
	screen->size = { framebuffer->GetSpec().width, framebuffer->GetSpec().height };
	screen->bound = false;
	screen->slot = 0;
	screen->alive = true;
	screen->multisampled = false;
	screen->always_rebind = true;

	std::string screen_name = "m_screen";
	renderer2d::get_texture_library()->Add(screen_name, screen);
	renderer2d::get_subtexture_library()->create("s_screen", screen_name, *screen, { 0,0 }, { screen->size.x, screen->size.y }, { 1,1 });

	std::shared_ptr<Texture_Data> screen2 = std::make_shared<Texture_Data>(Texture_Data());
	screen2->texture_id = framebuffer->get_color_attachment_renderer_id(0);
	screen2->size = { framebuffer->GetSpec().width, framebuffer->GetSpec().height };
	screen2->bound = false;
	screen2->slot = 0;
	screen2->alive = true;
	screen2->multisampled = false;
	screen2->always_rebind = true;

	std::string screen_name2 = "m_screen2";
	renderer2d::get_texture_library()->Add(screen_name2, screen2);
	renderer2d::get_subtexture_library()->create("s_screen2", screen_name2, *screen2, { 0,0 }, { screen2->size.x, screen2->size.y }, { 1,1 });

	std::shared_ptr<Texture_Data> pos = std::make_shared<Texture_Data>(Texture_Data());
	pos->texture_id = framebuffer->get_color_attachment_renderer_id(0);
	pos->size = { framebuffer->GetSpec().width, framebuffer->GetSpec().height };
	pos->bound = false;
	pos->slot = 0;
	pos->alive = true;
	pos->multisampled = false;
	pos->always_rebind = true;

	std::string pos_name = "m_pos";
	renderer2d::get_texture_library()->Add(pos_name, pos);
	renderer2d::get_subtexture_library()->create("s_pos", pos_name, *pos, { 0,0 }, { pos->size.x, pos->size.y }, { 1,1 });

	QUADrender_param pic;
	pic.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	pic.layer = 1;
	pic.size = { 1280.0f, 720.0f };
	pic.rotation = 0;
	pic.Texture = "s_screen";

	renderer2d::get_shader_library()->Load("MSAA", "res/shaders/MSAA.glsl");
	renderer2d::get_shader_library()->get("MSAA")->set_uniform_1i("u_samples", fbspec.samples);
	renderer2d::get_shader_library()->Load("Lighting", "res/shaders/Lighting.glsl");
	renderer2d::get_shader_library()->get("Lighting")->set_uniform_mat_4f("u_view_proj", m_camera->get_camera().Get_View_Projection_Matrix());

	int samplers[32];
	for (int i = 0; i < 32; i++)
	{
		samplers[i] = i;
	}

	renderer2d::get_shader_library()->get("MSAA")->set_uniform_1iv("u_textures", 32, samplers);
	renderer2d::get_shader_library()->get("Lighting")->set_uniform_1iv("u_textures", 32, samplers);
	renderer2d::get_shader_library()->get("Lighting")->set_uniform_1i("u_lights", 1);

	Light test(Light::LIGHT_TYPE::POINT_LIGHT, 0.2f, {0.05,0.0,0.5 }, { 1.0f, 0.0f, 0.0f });
	update_screen_details("Lighting");
	test.set_aspect_ratio(1280.0f, 720.0f);
	test.send_to_shader("Lighting");

	//temp geo code
	srand(time(NULL));
	std::vector<Geometry_Vertex> verticies;

	Geometry_Vertex v1;
	v1.position = { -1.0f, -0.5f };
	verticies.push_back(v1);
	Geometry_Vertex v2;
	v2.position = { -1.0f, 0.5f };
	verticies.push_back(v2);
	Geometry_Vertex v3;
	v3.position = { -0.5f, 1.0f };
	verticies.push_back(v3);
	Geometry_Vertex v4;
	v4.position = { 0.5f, 1.0f };
	verticies.push_back(v4);
	Geometry_Vertex v5;
	v5.position = { 1.0f, 0.5f};
	verticies.push_back(v5);
	Geometry_Vertex v6;
	v6.position = {	1.0f, -0.5f };
	verticies.push_back(v6);
	Geometry_Vertex v7;
	v7.position = { 0.5f, -1.0f };
	verticies.push_back(v7);
	Geometry_Vertex v8;
	v8.position = { -0.5f, -1.0f };
	verticies.push_back(v8);


	Geometry geo_test(verticies);
	geo_test.set_layer(2);
	geo_test.set_scale(200.0f);


	while (m_running)
	{
		renderer2d::clear();

		float c_time = glfwGetTime();
		Timestep time = c_time - m_last_frame_time;
		m_last_frame_time = c_time;
		
		renderer2d::update_quad_shader("Quad");
		renderer2d::Begin_Scene(m_camera->get_camera());

		if (!m_minimized)
		{
			m_camera->On_Update(time);

			pic.Texture = "s_screen";
			
			QUADrender_param square;
			square.color = { 0.5f, 0.5f, 0.5f, 0.0f };
			square.layer = 1;
			square.rotation = 0;
			square.size = { 60.0f, 60.0f };
			square.Texture = "";

			for (int i = 0; i < 5; i++)
			{
				for (int e = 0; e < 5; e++)
				{
					float x = -120 + (i * 60.0f);
					float y = 120 - (e * 60.0f);
					if (!(i == 2 && e == 2))
					{
						renderer2d::draw_quad({ x, y }, square);
					}
				}
			}

			QUADrender_param square2;
			square2.color = { 0.5f, 0.8f, 0.2f, 1.0f };
			square2.layer = 1;
			square2.rotation = 0;
			square2.size = { 60.0f, 60.0f };
			square2.Texture = "";

			for (int i = 0; i < 7; i++)
			{
				for (int e = 0; e < 7; e++)
				{
					float x = -180 + (e * 60.0f);
					float y = 180 - (i * 60.0f);
					if (i == 0)
					{
						renderer2d::draw_quad({ x,y }, square2);
					}
					else if (i > 0 && (e == 0 || e == 6))
					{
						renderer2d::draw_quad({ x,y }, square2);
					}
					else if (i == 6)
					{
						renderer2d::draw_quad({ x,y }, square2);
					}
				}
			}

			QUADrender_param square3;
			square3.color = { 0.9f, 0.2f, 0.4f, 0.5f };
			square3.layer = 1;
			square3.rotation = 0;
			square3.size = { 60.0f, 60.0f };
			square3.Texture = "";

			renderer2d::draw_quad({ 0,0 }, square3);

			//renderer2d::draw_polygon(geo_test);

			if (Framebufferspec spec = framebuffer->GetSpec(); spec.width != m_window->Get_Width() || spec.height != m_window->Get_Height())
			{
				framebuffer->Resize((uint32_t)m_window->Get_Width(), (uint32_t)m_window->Get_Height());
				framebuffer2->Resize((uint32_t)m_window->Get_Width(), (uint32_t)m_window->Get_Height());
				pic.size = { (uint32_t)m_window->Get_Width(), (uint32_t)m_window->Get_Height() };
			}


			framebuffer->Bind();
			framebuffer->clear_attachment(0, 0);
			framebuffer->clear_attachment(1, 0);
			framebuffer->clear_attachment(2, 0);
			glClear(GL_COLOR_BUFFER_BIT);
			renderer2d::End_Scene();

			framebuffer->Unbind();

			//renderer2d::get_texture_library()->get("m_screen")->texture_id = framebuffer->get_color_attachment_renderer_id(0);
			//renderer2d::get_texture_library()->get("m_screen")->texture_id = framebuffer->get_depth_attachment_renderer_id();

			/*
			glm::mat4 mvp = glm::ortho(0.0f, 16.0f / 9.0f, 0.0f, 1.0f, -1.0f, 1.0f);
			renderer2d::get_shader_library()->get("MSAA")->set_uniform_mat_4f("u_view_proj", mvp);

			framebuffer->Unbind();

			renderer2d::update_quad_shader("MSAA");

			renderer2d::draw_quad({ 0.5f, 0.5f }, pic);
			renderer2d::draw();
			*/

			//PostProcessing::MSAA("MSAA", pic, framebuffer->GetSpec(), framebuffer2);
			
			//renderer2d::get_texture_library()->get("m_screen")->texture_id = framebuffer->get_color_attachment_renderer_id(2);
			//PostProcessing::MSAA("MSAA", pic, framebuffer->GetSpec(), framebuffer3);

			//glm::mat4 mvp = glm::ortho(0.0f, 16.0f / 9.0f, 0.0f, 1.0f, -1.0f, 1.0f);
			//renderer2d::get_shader_library()->get("Lighting")->set_uniform_mat_4f("u_view_proj", mvp);
			
			renderer2d::get_texture_library()->get("m_screen2")->texture_id = framebuffer->get_color_attachment_renderer_id(0);
			renderer2d::get_texture_library()->get("m_pos")->texture_id = framebuffer->get_color_attachment_renderer_id(2);
			//renderer2d::get_texture_library()->get("m_screen2")->texture_id = framebuffer->get_color_attachment_renderer_id(1);
			
			//renderer2d::unbind_texture("m_screen2");
			//int albedo = (int)renderer2d::bind_texture("m_screen2");
			//int position = (int)renderer2d::bind_texture("m_pos");
			////int depth = (int)renderer2d::bind_texture("m_screen2");
			//
			//renderer2d::get_shader_library()->get("Lighting")->set_uniform_1i("gPosition", position);
			//renderer2d::get_shader_library()->get("Lighting")->set_uniform_1i("gAlbedo", albedo);
			//
			pic.Texture = "s_screen2";
			////pic.Texture = "";
			//
			//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			//
			//renderer2d::update_quad_shader("Lighting");
			////renderer2d::Begin_Scene(m_camera->get_camera());
			//renderer2d::draw_quad({ 0.5f, 0.5f }, pic);
			//renderer2d::End_Scene();

			PostProcessing::DefferedLighting("Lighting", pic, { "m_screen2", "m_pos" }, m_camera);
			
			pic.Texture = "s_screen";

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

	glViewport(0, 0, e.GetWidth(), e.GetHeight());

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
