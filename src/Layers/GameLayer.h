#pragma once

#include "Layer.h"

class GameLayer : public Layer
{
public:
	GameLayer();
	virtual ~GameLayer() override;

	virtual void On_Attach() override;
	virtual void On_Detach() override;
	virtual void On_Update(Timestep ts) override;
	virtual void On_ImGui_Render() override;
	virtual void On_Event(Events::Event& event) override;
};