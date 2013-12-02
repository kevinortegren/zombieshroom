#include "DebugDrawer.h"

#include <RootEngine/Render/Include/Renderer.h>

namespace Physics
{
	extern RootEngine::SubsystemSharedContext g_context;
	extern Render::RendererInterface* g_renderer;
	extern RootEngine::ResourceManagerInterface* g_resourceManager;
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
			color[i]  = p_color[i];
			//color[i] = 1;
		}
		
		Physics::g_renderer->AddLine(from, to , glm::vec4(color,1.0f));

		
		
	}
}

DebugDrawer::DebugDrawer()
{
	m_counter = 0;
	
}

DebugDrawer::~DebugDrawer()
{

}

void DebugDrawer::ClearDebugVectors()
{
	m_debugVectors.clear();
}
