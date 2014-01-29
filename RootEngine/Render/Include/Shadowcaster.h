#pragma once

#include <glm/glm.hpp>

namespace Render
{
	struct Shadowcaster 
	{
		//Low-level Shadowcaster for Renderer
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrices[3]; //Identity matrix in ortho shadows
	};
}