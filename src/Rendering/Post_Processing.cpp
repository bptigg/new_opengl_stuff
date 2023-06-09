#include "Post_Processing.h"
#include "../Core/Application.h"

void PostProcessing::MSAA(std::string msaa_shader, QUADrender_param& draw_cmd, std::shared_ptr<Framebuffer> output_framebuffer)
{
	if (output_framebuffer != nullptr)
	{
		output_framebuffer->Bind();
		for (int i = 0; i < output_framebuffer->GetSpec().Attachments.Attachments.size(); i++)
		{
			output_framebuffer->clear_attachment(i, -1);
		}
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
