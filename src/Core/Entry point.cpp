#include <iostream>

#include "Application.h"

extern Application* create_application(Application_cmd_args args);

int main(int argc, char** argv)
{
	auto app = create_application({ argc, argv });
	app->Run();
	delete app;
	std::cin.get();
}