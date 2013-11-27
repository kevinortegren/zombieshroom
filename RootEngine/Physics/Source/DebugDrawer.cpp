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
		}
		
		Physics::g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Line %d between (%f, %f, %f) and (%f, %f, %f)", m_counter, from[0], from[1], from[2], to[0], to[1], to[2]);
		m_counter++;
		Render::Vertex1P1C vertices[2];
		vertices[0].m_pos = from;
		vertices[0].m_color = glm::vec4(color, 1.0f);
		vertices[1].m_pos = to;
		vertices[1].m_color = glm::vec4(color, 1.0f);
		unsigned int indices[2] = {0,1};
		std::shared_ptr<Render::MeshInterface> mesh = Physics::g_renderer->CreateMesh();
		mesh->Init(vertices, 2, indices, 2);
		Render::Uniforms uniforms;
		uniforms.m_normal = glm::mat4(1);
		uniforms.m_world = glm::mat4(1);


		Render::RenderJob job;
		job.m_mesh = mesh;
		job.m_uniforms = &uniforms;
		job.m_effect = Physics::g_resourceManager->GetEffect("test");

		Physics::g_renderer->AddRenderJob(&job);
		
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
