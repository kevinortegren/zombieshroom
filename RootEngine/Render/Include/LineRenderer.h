#pragma once

#include <RootEngine/Render/Include/Effect.h>
#include <RootEngine/Render/Include/Mesh.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace Render
{
	struct Line
	{
		Line(glm::vec3 p_fromPoint, glm::vec3 p_toPoint, glm::vec4 p_color)
		{
			m_fromPoint = p_fromPoint;
			m_toPoint = p_toPoint;
			m_color = p_color;
		}
		glm::vec3 m_fromPoint;
		glm::vec3 m_toPoint;
		glm::vec4 m_color;
	};

	class GLRenderer;

	class LineRenderer
	{
	public:
		void Init(GLRenderer* p_renderer);
		void AddLine(Line& line);
		void RenderLines();

	private:
		std::vector<Line> m_lines;
		Mesh m_lineMesh;
		std::shared_ptr<TechniqueInterface> m_debugTech;
	};
}