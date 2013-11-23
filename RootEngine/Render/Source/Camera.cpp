#include <Camera.h>

void Camera::Initialize(glm::vec3 p_position, glm::vec3 p_target, glm::vec3 p_up,
		float p_fov, float p_near, float p_far)
{
	m_position = p_position;
	m_target = p_target;
	m_up = p_up;

	LookAt();
	PerspectiveProjection(p_fov, p_near, p_far);
}

void Camera::LookAt() 
{
	m_view = glm::lookAt(m_position, m_target, m_up);
}

void Camera::PerspectiveProjection(float p_fov, float p_near, float p_far)
{
	m_projection = glm::perspectiveFov<float>(p_fov, 800, 600, p_near, p_far);
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

