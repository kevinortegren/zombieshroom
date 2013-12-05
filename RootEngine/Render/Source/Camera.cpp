#include <Camera.h>

namespace Render
{

	void Camera::Initialize(glm::vec3 p_position, glm::vec3 p_target, glm::vec3 p_up,
			float p_fov, float p_near, float p_far, int p_width, int p_height)
	{
		m_position = p_position;
		m_target = p_target;
		m_up = p_up;

		LookAt();
		PerspectiveProjection(p_fov, p_near, p_far, p_width, p_height);
	}

	void Camera::LookAt() 
	{
		m_view = glm::lookAt(m_position, m_target, m_up);
	}

	void Camera::PerspectiveProjection(float p_fov, float p_near, float p_far, int p_width, int p_height)
	{
		m_projection = glm::perspectiveFov<float>(p_fov, (float)p_width, (float)p_height, p_near, p_far);
	}

	void Camera::Update()
	{
		LookAt();
	}

	void Camera::SetTarget(glm::vec3 p_target)
	{
		m_target = p_target;
	}

	void Camera::SetPosition(glm::vec3 p_position)
	{
		m_position = p_position;
	}

}
