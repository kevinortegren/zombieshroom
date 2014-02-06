#pragma once

#include <glm/glm.hpp>

namespace Render
{
	struct Shadowcaster 
	{
		//Low-level Shadowcaster for Renderer
		glm::mat4 m_viewMatrices[4];
		glm::mat4 m_projectionMatrices[4]; //Identity matrix in ortho shadows
		glm::mat4 m_viewProjections[4];
	};
}