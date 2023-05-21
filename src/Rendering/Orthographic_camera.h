#pragma once

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

class Orthographic_camera
{
public:
	Orthographic_camera(float left, float right, float bottom, float top);

	void set_projection(float left, float right, float bottom, float top);

	void Set_Position(const glm::vec3& position) { 
		update_view_matrix = true;
		m_Position = position; 
		Recalculate_View_Matrix();
	}

	const glm::vec2& get_Position() const { return m_Position; }

	const glm::mat4& Get_Projection_Matrix() const { return m_Projection_Matrix; }
	const glm::mat4& Get_View_Matrix() const { return m_View_Matrix; }
	const glm::mat4& Get_View_Projection_Matrix() const { return m_View_Projection_Matrix_cache; }

private:
	void Recalculate_View_Matrix();
private:
	glm::mat4 m_Projection_Matrix;
	glm::mat4 m_View_Matrix;
	glm::mat4 m_View_Projection_Matrix_cache;

	glm::vec2 m_Position;

	bool update_view_matrix = true;
};

