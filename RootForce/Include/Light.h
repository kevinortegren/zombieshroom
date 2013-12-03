#pragma once

#include <Utility\ECS\Include\Component.h>
#include <glm/glm.hpp>
#include <yaml-cpp\yaml.h>
#include <iostream>

namespace RootForce
{
	struct PointLight : public ECS::Component<PointLight>
	{
		glm::vec4 m_color;
		glm::vec3 m_attenuation;
		float m_range;
	};
}