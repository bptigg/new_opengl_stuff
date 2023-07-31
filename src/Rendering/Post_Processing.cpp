#include "Post_Processing.h"
#include "../Core/Application.h"


static bool MSAA_MULTISAMPLING_FALSE = false;

void PostProcessing::MSAA(std::string msaa_shader, QUADrender_param& draw_cmd, const Framebufferspec& geometry_framebuffer, std::shared_ptr<Framebuffer> output_framebuffer)
{
	if (output_framebuffer != nullptr)
	{
		output_framebuffer->Bind();
		for (int i = 0; i < output_framebuffer->GetSpec().Attachments.Attachments.size(); i++)
		{
			output_framebuffer->clear_attachment(i, 0);
		}
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	if (geometry_framebuffer.samples == 1)
	{
		msaa_shader = "Quad";
		if (!MSAA_MULTISAMPLING_FALSE)
		{
			Log::warning("MSAA POST PROCESSING STEP CALLED ON A NON MULTISAMPLED FRAMEBUFFER");
			MSAA_MULTISAMPLING_FALSE = true;
		}
	}

	float window_dim[2] = { Application::Get().Get_window().Get_Width(), Application::Get().Get_window().Get_Height()};
	float aspect_ratio = window_dim[0] / window_dim[1];

	glm::mat4 mvp = glm::ortho(0.0f, aspect_ratio, 0.0f, 1.0f, -1.0f, 1.0f);
	renderer2d::get_shader_library()->get(msaa_shader)->set_uniform_mat_4f("u_view_proj", mvp);
	
	renderer2d::update_quad_shader(msaa_shader);
	renderer2d::draw_quad({ 0.5f,0.5f }, draw_cmd);
	renderer2d::draw();

	if (output_framebuffer != nullptr)
	{
		output_framebuffer->Unbind();
	}
}

void PostProcessing::DefferedLighting(std::string lighting_shader, QUADrender_param& draw_cmd, LightingData light_data, Camera_Controller*& camera, std::shared_ptr<Framebuffer> output_framebuffer)
{
	if (output_framebuffer != nullptr)
	{
		output_framebuffer->Bind();
		for (int i = 0; i < output_framebuffer->GetSpec().Attachments.Attachments.size(); i++)
		{
			output_framebuffer->clear_attachment(i, 0);
		}
		glClear(GL_COLOR_BUFFER_BIT);
	}

	if (camera->get_camera().get_update() == true)
	{
		renderer2d::get_shader_library()->get(lighting_shader)->set_uniform_mat_4f("u_view_proj", camera->get_camera().Get_View_Projection_Matrix());
		renderer2d::get_shader_library()->get(lighting_shader)->set_uniform_1f("zoom_level", camera->Get_Zoom_Level());
		camera->get_camera().got_update();
	}

	int albedo = (int)renderer2d::bind_texture(light_data.AlbedoTexture);
	int position = (int)renderer2d::bind_texture(light_data.PositionTexture);

	renderer2d::get_shader_library()->get(lighting_shader)->set_uniform_1i("gPosition", position);
	renderer2d::get_shader_library()->get(lighting_shader)->set_uniform_1i("gAlbedo", albedo);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	renderer2d::update_quad_shader(lighting_shader);
	renderer2d::draw_quad({ 0.5f, 0.5f }, draw_cmd);
	renderer2d::End_Scene();
}
