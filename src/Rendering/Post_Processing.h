#include "shader.h"
#include "Framebuffer.h"
#include "renderer2d.h"

extern class Camera_Controller;

namespace PostProcessing
{

	struct LightingData
	{
		std::string AlbedoTexture;

		std::string PositionTexture;
	};

	void MSAA(std::string msaa_shader, QUADrender_param& draw_cmd, const Framebufferspec& geometry_framebuffer, std::shared_ptr<Framebuffer> output_framebuffer = nullptr);
	void DefferedLighting(std::string lighting_shader, QUADrender_param& draw_cmd, LightingData light_data, Camera_Controller*& camera, std::shared_ptr<Framebuffer> output_framebuffer = nullptr);
}