#include "Camera_Controller.h"

//update camerar controller to use relative coordinates rather than actual i.e glm::mat4 mvp = glm::ortho(0.0f, 16.0f/9.0f, 0.0f, 1.0f, -1.0f, 1.0f);


Camera_Controller::Camera_Controller(int width, int height, bool rotation)
	:m_aspect_ratio((float)width / (float)height), m_camera(-width / 2.0f * m_zoom_level, width / 2.0f * m_zoom_level, -height / 2.0f * m_zoom_level, height / 2.0f * m_zoom_level)
{
	m_rotation = rotation;
	m_camera_translation_speed = 100.0f;
	m_block = false;
	m_resolution = { width, height };
}

void Camera_Controller::On_Update(Timestep ts)
{
	m_camera.Set_Position(m_camera_position);
	m_camera_translation_speed = 100.0f * m_zoom_level;
	m_frame_time = ts;
	//m_camera_translation_speed = 100.0f;
	//m_camera_translation_speed = m_zoom_level * m_camera_translation_speed;
	//m_frame_time = ts;
	//bool changed = false;
	//if (!m_block)
	//{
	//	if (Input::Is_Key_Pressed(CS_KEY_A))
	//	{
	//		m_camera_position.x -= cos(glm::radians(m_camera_rotation)) * m_camera_translation_speed * ts;
	//		m_camera_position.y -= sin(glm::radians(m_camera_rotation)) * m_camera_translation_speed * ts;
	//		changed = true;
	//	}
	//	else if (Input::Is_Key_Pressed(CS_KEY_D))
	//	{
	//		m_camera_position.x += cos(glm::radians(m_camera_rotation)) * m_camera_translation_speed * ts;
	//		m_camera_position.y += sin(glm::radians(m_camera_rotation)) * m_camera_translation_speed * ts;
	//		changed = true;
	//	}
	//
	//	if (Input::Is_Key_Pressed(CS_KEY_W))
	//	{
	//		m_camera_position.x += -sin(glm::radians(m_camera_rotation)) * m_camera_translation_speed * ts;
	//		m_camera_position.y += cos(glm::radians(m_camera_rotation)) * m_camera_translation_speed * ts;
	//		changed = true;
	//	}
	//	else if (Input::Is_Key_Pressed(CS_KEY_S))
	//	{
	//		m_camera_position.x -= -sin(glm::radians(m_camera_rotation)) * m_camera_translation_speed * ts;
	//		m_camera_position.y -= cos(glm::radians(m_camera_rotation)) * m_camera_translation_speed * ts;
	//		changed = true;
	//	}
	//}
	//
	//if (m_rotation)
	//{
	//	if (Input::Is_Key_Pressed(CS_KEY_Q))
	//		m_camera_rotation += m_rotation_speed;
	//	if (Input::Is_Key_Pressed(CS_KEY_E))
	//		m_camera_rotation -= m_rotation_speed;
	//
	//	if (m_camera_rotation > 180.0f)
	//		m_camera_rotation -= 360.0f;
	//	else if (m_camera_rotation <= -180.0f)
	//		m_camera_rotation += 360.0f;
	//	changed = true;
	//}
	//
	//if (changed)
	//{
	//	m_camera.Set_Position(m_camera_position * m_zoom_level);
	//}

}

void Camera_Controller::On_Event(Events::Event& e)
{
	Events::Event_Dispatcher dispatcher(e);
	dispatcher.Dispatch<Events::Mouse_Scrolled_Event>(std::bind(&Camera_Controller::on_mouse_scroll, this, std::placeholders::_1));
	dispatcher.Dispatch<Events::Window_Resize_Event>(std::bind(&Camera_Controller::on_window_resize, this, std::placeholders::_1));
	dispatcher.Dispatch<Events::Key_Pressed_Event>(std::bind(&Camera_Controller::on_key_event, this, std::placeholders::_1));
}

void Camera_Controller::OnResize(float width, float height)
{
	//m_aspect_ratio = width / height;
	m_camera.set_projection(-width/2.0f * m_zoom_level, width/2.0f * m_zoom_level, -height/2.0f * m_zoom_level, height/2.0f * m_zoom_level);
	m_resolution = { width, height };
}

bool Camera_Controller::on_key_event(Events::Key_Pressed_Event& e)
{
	if (e.Get_Key_Code() == CS_KEY_R)
	{
		//m_camera_position = { 0.0f,0.0f,0.0f };
		//m_camera.Set_Position(m_camera_position);
		//m_zoom_level = 1.0f;
		//m_camera.set_projection((-m_resolution.x / 2.0f) * m_zoom_level, (m_resolution.x / 2.0f) * m_zoom_level, (-m_resolution.y / 2.0f) * m_zoom_level, (m_resolution.y / 2.0f) * m_zoom_level);
		return true;
	}
	else
	{
		if (e.Get_Key_Code() == CS_KEY_A)
		{
			m_camera_position.x -= cos(glm::radians(m_camera_rotation)) * m_camera_translation_speed * m_frame_time;
			m_camera_position.y -= sin(glm::radians(m_camera_rotation)) * m_camera_translation_speed * m_frame_time;
		}
		else if (e.Get_Key_Code() == CS_KEY_D)
		{
			m_camera_position.x += cos(glm::radians(m_camera_rotation)) * m_camera_translation_speed * m_frame_time;
			m_camera_position.y += sin(glm::radians(m_camera_rotation)) * m_camera_translation_speed * m_frame_time;
		}
		else if (e.Get_Key_Code() == CS_KEY_W)
		{
			m_camera_position.x += -sin(glm::radians(m_camera_rotation)) * m_camera_translation_speed * m_frame_time;
			m_camera_position.y += cos(glm::radians(m_camera_rotation)) * m_camera_translation_speed * m_frame_time;
		}
		else if (e.Get_Key_Code() == CS_KEY_S)
		{
			m_camera_position.x -= -sin(glm::radians(m_camera_rotation)) * m_camera_translation_speed * m_frame_time;
			m_camera_position.y -= cos(glm::radians(m_camera_rotation)) * m_camera_translation_speed * m_frame_time;
		}

		if (m_rotation)
		{
			if (Input::Is_Key_Pressed(CS_KEY_Q))
				m_camera_rotation += m_rotation_speed;
			if (Input::Is_Key_Pressed(CS_KEY_E))
				m_camera_rotation -= m_rotation_speed;

			if (m_camera_rotation > 180.0f)
				m_camera_rotation -= 360.0f;
			else if (m_camera_rotation <= -180.0f)
				m_camera_rotation += 360.0f;
		}

		m_camera.Set_Position(m_camera_position);
		m_camera_translation_speed = 100.0f * m_zoom_level;
		return false;
	}

	return false;
}

bool Camera_Controller::on_mouse_scroll(Events::Mouse_Scrolled_Event& e)
{
	if (!m_block)
	{
		m_zoom_level -= e.GetYOffset() * 0.125f;
		m_zoom_level = std::max(m_zoom_level, 0.25f);
		m_zoom_level = (m_zoom_level <= 3) ? m_zoom_level : 3;
		m_camera.set_projection(-m_resolution.x / 2.0f * m_zoom_level, m_resolution.x / 2.0f * m_zoom_level, -m_resolution.y / 2.0f * m_zoom_level, m_resolution.y / 2.0f * m_zoom_level);
	}
	return false;
}

bool Camera_Controller::on_window_resize(Events::Window_Resize_Event& e)
{
	OnResize((float)e.GetWidth(), (float)e.GetHeight());
	return false;
}
