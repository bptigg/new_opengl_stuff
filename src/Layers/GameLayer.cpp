
#include "GameLayer.h"

GameLayer::GameLayer()
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
	return;
}

void GameLayer::On_Detach()
{
	return;
}

void GameLayer::On_Update(Timestep ts)
{
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
