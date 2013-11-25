#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera
{
public:
	
	void Initialize(glm::vec3 p_position, glm::vec3 p_target, glm::vec3 p_up, float p_fov, float p_near, float p_far);

	const glm::mat4& GetView() const { return m_view; }
	const glm::mat4& GetProjection() const { return m_projection; }
	const glm::vec3& GetPosition() const { return m_position; }
	const glm::vec3& GetTarget() const { return m_target; }

	void SetTarget(glm::vec3 p_target);
	void SetPosition(glm::vec3 p_position);

	void Update();

private:

	void LookAt();
	void PerspectiveProjection(float p_fov, float p_near, float p_far);

	glm::mat4 m_view;
	glm::mat4 m_projection;
	glm::vec3 m_position;
	glm::vec3 m_target;
	glm::vec3 m_up;
};
