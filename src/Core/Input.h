#pragma once

#include <glm/glm.hpp>

#include "../Core/Key_Codes.h"

class Input
{
public:
	static bool Is_Key_Pressed(int key);

	static bool Is_MouseButton_Pressed(int button);
	static glm::vec2 Get_Mouse_Position();
	static float Get_Mouse_X();
	static float Get_Mouse_Y();

	static glm::vec2 Get_Window_Size();
};