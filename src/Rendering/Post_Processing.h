#include "shader.h"
#include "Framebuffer.h"
#include "renderer2d.h"

namespace PostProcessing
{
	void MSAA(std::string msaa_shader, QUADrender_param& draw_cmd, const Framebufferspec& geometry_framebuffer, std::shared_ptr<Framebuffer> output_framebuffer = nullptr);
}