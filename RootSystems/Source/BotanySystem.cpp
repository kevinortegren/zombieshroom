#include <RootSystems/Include/BotanySystem.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>

namespace RootForce
{
	void BotanySystem::Initialize()
	{
		// Load rendering effects.
		m_effect = m_engineContext->m_resourceManager->LoadEffect("Botany");

		m_material = m_engineContext->m_renderer->CreateMaterial("Botany");
		m_material->m_textures[Render::TextureSemantic::DIFFUSE] = m_engineContext->m_resourceManager->LoadTexture("LightGreen2", Render::TextureType::TEXTURE_2D);
		m_material->m_effect = m_effect;

		m_meshes.resize(100);
		for(int i = 0; i < 100; i++)
		{
			m_meshes[i] = m_engineContext->m_renderer->CreateMesh();
			m_meshes[i]->SetPrimitiveType(GL_PATCHES);
		}
	}

	void BotanySystem::SetQuadTree(QuadTree* p_quadTree)
	{
		m_quadTree = p_quadTree;
	}

	void BotanySystem::Process()
	{
		// Get eye camera.
		ECS::Entity* entity = m_world->GetTagManager()->GetEntityByTag("Camera");
		RootForce::Frustum* frustrum = &m_world->GetEntityManager()->GetComponent<RootForce::Camera>(entity)->m_frustum;
		RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(entity);

		// Cull terrain chunks.
		m_quadTree->m_culledNodes.clear();
		m_quadTree->CullNodes(frustrum, m_quadTree->GetRoot());

		int j = 0;
		for(auto itr = m_quadTree->m_culledNodes.begin(); itr != m_quadTree->m_culledNodes.end(); ++itr)
		{
			AABB bounds = (*itr)->GetBounds();
			glm::vec3 center = bounds.GetCenter();
			float d = glm::distance(transform->m_position, center);
			
			// Render terrain entities.
			for(auto entity = (*itr)->m_terrainEntityIndices.begin(); entity != (*itr)->m_terrainEntityIndices.end(); ++entity)
			{
				RootForce::Renderable* renderable = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(m_quadTree->m_entities[(*entity)]);

				Render::RenderJob job;
				job.m_mesh = m_meshes[j];

				job.m_mesh->SetVertexBuffer(renderable->m_model->m_meshes[0]->GetVertexBuffer());
				job.m_mesh->SetElementBuffer(renderable->m_model->m_meshes[0]->GetElementBuffer());
				job.m_mesh->SetVertexAttribute(renderable->m_model->m_meshes[0]->GetVertexAttribute());
				

				job.m_mesh->SetPrimitiveType(GL_PATCHES);
				job.m_material = m_material;
				//job.m_mesh->SetWireFrame(true);

				// Assign weight map.
				job.m_material->m_textures[Render::TextureSemantic::TEXTUREMAP] = renderable->m_material->m_textures[Render::TextureSemantic::TEXTUREMAP];

				job.m_params = renderable->m_params;
				job.m_flags = Render::RenderFlags::RENDER_IGNORE_CASTSHADOW;

				m_engineContext->m_renderer->AddRenderJob(job);
				j++;
			}
		}
	}
}