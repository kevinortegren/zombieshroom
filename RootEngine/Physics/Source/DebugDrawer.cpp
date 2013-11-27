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
		glm::vec3 from , to, color;
		for(int i = 0; i < 3; i++)
		{
			from[i] = p_from[i];
			to[i] = p_to[i];
			color[i] = p_color[i];
		}
		//std::shared_ptr<Render::MeshInterface> temp = m_renderer->CreateMesh();

		Physics::g_renderer->DrawLine(from, to, color);
		
		
	}
}

DebugDrawer::DebugDrawer()
{
	
}

DebugDrawer::~DebugDrawer()
{

}
