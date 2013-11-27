#include "DebugDrawer.h"

#include <RootEngine/Render/Include/Renderer.h>

namespace Physics
{
	extern Render::RendererInterface* g_renderer;
}

void DebugDrawer::drawLine( const btVector3& p_from, const btVector3& p_to, const btVector3& p_color )
{
	if(Physics::g_renderer != NULL)
	{
		glm::vec3 from , to;
		for(int i = 0; i < 3; i++)
		{
			from[i] = p_from[i];
			to[i] = p_to[i];
		}
		std::shared_ptr<Render::MeshInterface> mesh = Physics::g_renderer->CreateMesh();
		m_debugVectors.push_back(from);
		m_debugVectors.push_back(to);		
	}
}

DebugDrawer::DebugDrawer()
{
	
}

DebugDrawer::~DebugDrawer()
{

}

void DebugDrawer::ClearDebugVectors()
{
	m_debugVectors.clear();
}
