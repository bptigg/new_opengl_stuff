#pragma once

#include "../Core/Timestep.h"
#include "../Events/Event.h"

#include <string>

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1);

extern class Orthographic_camera;

class Layer
{
public:
	using Event_Call_back_fn = std::function<void(Events::Event&)>;
public:
	Layer(const std::string& name = "Layer")
		:m_debug_name(name)
	{

	}

	virtual ~Layer() = default;
	virtual void On_Attach() = 0;
	virtual void On_Detach() = 0;
	virtual void On_Update(Timestep ts) = 0;
	virtual void On_ImGui_Render() = 0;
	virtual void On_Event(Events::Event& event) = 0;

	inline void Set_Event_Callback(const Event_Call_back_fn& callback) { Event_Call_Back = callback; }
	virtual void add_orthographic_camera(Orthographic_camera& camera) { return; };

#if _DEBUG
	inline const std::string& get_name() { return m_debug_name; }
#endif

protected:
	Event_Call_back_fn Event_Call_Back;
#if _DEBUG
	std::string	m_debug_name;
#endif
};

