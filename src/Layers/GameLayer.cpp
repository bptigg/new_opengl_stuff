
#include "GameLayer.h"

#include "../Core/Application.h"

GameLayer::GameLayer(int screen_width, int screen_height, int samples)
	:m_screen({screen_width, screen_height}), m_samples(samples)
{

}

GameLayer::~GameLayer()
{
#if _DEBUG
	std::cout << "Layer: " << m_debug_name << " Deleted\n";
#endif
}

void GameLayer::On_Attach()
{
	Framebufferspec fbspec;
	fbspec.Attachments = { FramebufferTextureFormat::RGBA8 , FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
	fbspec.width = m_screen.first;
	fbspec.height = m_screen.second;
	fbspec.samples = m_samples;
	geometry_buffer = Framebuffer::Create(fbspec);

	screen_texture = std::make_shared<Texture_Data>(Texture_Data());
	screen_texture->texture_id = 0;
	screen_texture->size = { m_screen.first, m_screen.second };
	screen_texture->bound = false;
	screen_texture->slot = 0;
	screen_texture->alive = true;
	screen_texture->multisampled = true;

	std::string name = "m_screen";
	renderer2d::get_texture_library()->Add(name, screen_texture);
	renderer2d::get_subtexture_library()->create("s_screen", name, *screen_texture, { 0,0 }, screen_texture->size, { 1,1 });

	m_screen_quad.color = { 1.0, 1.0, 1.0, 1.0 };
	m_screen_quad.layer = 1;
	m_screen_quad.size = screen_texture->size;
	m_screen_quad.rotation = 0;
	m_screen_quad.Texture = "s_screen";

	return;
}

void GameLayer::On_Detach()
{
	return;
}

void GameLayer::On_Update(Timestep ts)
{
	glm::vec2 window_size = { Application::Get().Get_window().Get_Width(), Application::Get().Get_window().Get_Height() };
	if (Framebufferspec spec = geometry_buffer->GetSpec(); spec.width != window_size.x || spec.height != window_size.y)
	{
		geometry_buffer->Resize((uint32_t)window_size.x, (uint32_t)window_size.y);
		screen_texture->size = window_size;
		renderer2d::get_texture_library()->update_texture("m_screen", screen_texture);
		m_screen_quad.size = window_size;
	}
	return;
}

void GameLayer::On_ImGui_Render()
{
	return;
}

void GameLayer::On_Event(Events::Event& event)
{
	return;
}
