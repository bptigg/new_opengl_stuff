#pragma once

#include "Window.h"
#include "../Events/Event.h"
#include "../Events/Application_Event.h"

#include "../Rendering/Camera_Controller.h"

#include "Timestep.h"


struct Application_cmd_args
{
	int count = 0;
	char** args = nullptr;

	const char* operator[](int index) const { return args[index]; }
};

struct Application_spec
{
	std::string Name = "Default Application";
	std::string Working_Directory;
	Application_cmd_args cmd_args;
};

class Application
{
public:
	Application(const Application_spec& spec);
	~Application();

	void on_event(Events::Event& e);

	//void push_layer(Layer* layer);
	//void push_overlay(Layer* layer);

	const Camera_Controller* Get_Camera() const { return m_camera; }
	Camera_Controller*& Get_Camera() { return m_camera; }
	Window& Get_window() { return *m_window; }
	static Application& Get() { return *s_instance; }


	void Run();
private:
	bool on_window_close(Events::Window_Close_Event& e);
	bool on_window_resize(Events::Window_Resize_Event& e);
private:
	Application_spec m_spec;
	Window* m_window;

	bool m_running = true;
	bool m_minimized = false;

	float m_last_frame_time = 0.0f;

	Camera_Controller* m_camera; //temporary

private:
	static Application* s_instance;
};

Application* create_application(Application_cmd_args args);