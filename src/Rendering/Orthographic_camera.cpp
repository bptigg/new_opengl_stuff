#include "Orthographic_camera.h"

Orthographic_camera::Orthographic_camera(float left, float right, float bottom, float top)
	:m_Projection_Matrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_View_Matrix(1.0f)
{
	m_View_Projection_Matrix_cache = m_Projection_Matrix * m_View_Matrix;
	update_view_matrix = false;
}

void Orthographic_camera::set_projection(float left, float right, float bottom, float top)
{
	m_Projection_Matrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	m_View_Projection_Matrix_cache = m_Projection_Matrix * m_View_Matrix;
	update_view_matrix = false;
}

void Orthographic_camera::Recalculate_View_Matrix()
{
	if (update_view_matrix)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(m_Position, 0.0f));

		m_View_Matrix = glm::inverse(transform);
		m_View_Projection_Matrix_cache = m_Projection_Matrix * m_View_Matrix;

		update_view_matrix = false;
	}
}
