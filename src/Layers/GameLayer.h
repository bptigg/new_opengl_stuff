#pragma once

#include "Layer.h"

#include "../Rendering/Framebuffer.h"

#include "../Rendering/SubTexture.h"
#include "../Rendering/renderer2d.h"

class GameLayer : public Layer
{
public:
	GameLayer(int screen_width = 1280, int screen_height = 720, int samples = 1);
	virtual ~GameLayer() override;

	virtual void On_Attach() override;
	virtual void On_Detach() override;
	virtual void On_Update(Timestep ts) override;
	virtual void On_ImGui_Render() override;
	virtual void On_Event(Events::Event& event) override;
private:
	std::shared_ptr<Framebuffer> geometry_buffer;
	std::shared_ptr<Texture_Data> screen_texture;

	std::pair<int, int> m_screen;
	int m_samples;

	QUADrender_param m_screen_quad;
	
};
