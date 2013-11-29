#pragma once

#include <ECS/Component.h>
#include <glm/glm.hpp>

namespace RootForce
{
	struct PointLight : public ECS::Component<PointLight>
	{
		glm::vec4 m_color;
		glm::vec3 m_attenuation;
		float m_range;
	};

	struct DirectionalLight : public ECS::Component<DirectionalLight>
	{
		glm::vec4 m_color;

	};
}