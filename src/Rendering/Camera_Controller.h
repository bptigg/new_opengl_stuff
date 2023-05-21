#pragma once

#include <glm/glm.hpp>

#include "Orthographic_camera.h"

#include "../Events/Application_Event.h"
#include "../Events/Mouse_Event.h"
#include "../Events/Key_Event.h"

#include "../Core/Input.h"

#include "../Core/Timestep.h"

class Camera_Lock
{
private:
	bool lock = false;
	uint32_t owner; 
public:
	bool Lock(uint32_t key)
	{
		if (lock)
		{
			return false;
		}

		lock = true;
		owner = key;
		return true;
	}

	bool Unlock(uint32_t key)
	{
		if (key == owner)
		{
			lock = false;
			return true;
		}
		return false;
	}
};

class Camera_Controller
{
public:
	Camera_Controller(float apsect_ratio, bool rotation = false);

	void On_Update(Timestep ts);
	void On_Event(Events::Event& e);

	void OnResize(float width, float height);

	Orthographic_camera& get_camera() { return m_camera; }
	const Orthographic_camera& get_camera() const { return m_camera; }

	float Get_Zoom_Level() const { return m_zoom_level; }
	void Set_Zoom_Level(float level) { m_zoom_level = level; }

	void Set_Resolution(glm::vec2 res) {
		m_resolution = res; m_camera.set_projection(-m_resolution.x / 2.0f, m_resolution.x / 2.0f, -m_resolution.y / 2.0f, m_resolution.y / 2.0f); m_camera_translation_speed = 5.0f;
	}

	const glm::vec3 get_position() const{ return m_camera_position; }
	inline void set_position() { m_camera_position = glm::vec3{ m_camera.get_Position(),0.0f }; }

	void block_camera(bool state, uint32_t key) {
		if (state == true)
		{
			if (m_lock.Lock(key))
			{
				m_block = true;
			}
		}
		if (state == false)
		{
			if (m_lock.Unlock(key) == true)
			{
				m_block = false;
			}
		}
	}

	bool get_block() { return m_block; }

private:
	bool on_mouse_scroll(Events::Mouse_Scrolled_Event& e);
	bool on_window_resize(Events::Window_Resize_Event& e);
	bool on_key_event(Events::Key_Pressed_Event& e);
private:
	float m_aspect_ratio;
	float m_zoom_level = 1.0f;
	Timestep m_frame_time;

	bool m_rotation;

	Orthographic_camera m_camera;

	glm::vec3 m_camera_position = { 0.0f, 0.0f, 0.0f };
	glm::vec2 m_resolution = { 0.0f, 0.0f };
	float m_camera_rotation = 0.0f;
	float m_camera_translation_speed = 1.0f;
	float m_rotation_speed = 180.0f;

	bool m_block;
	Camera_Lock m_lock;

};

