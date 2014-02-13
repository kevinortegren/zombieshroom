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

		m_meshes.resize(200);
		for(int i = 0; i < 200; i++)
		{
			m_meshes[i] = m_engineContext->m_renderer->CreateMesh();
			m_meshes[i]->SetPrimitiveType(GL_PATCHES);
		}

		m_distances.resize(200);
		for(int i = 0; i < 200; i++)
		{
			m_distances[i] = 0.0f;
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
		RootForce::Camera* camera = m_world->GetEntityManager()->GetComponent<RootForce::Camera>(entity);
		RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(entity);

		/*float n = 1.0f;
		float f = 10.0f;

		glm::mat4 modifiedProj = camera->m_projMatrix;
		modifiedProj[2][2] = (-f + n) / (f - n);
		modifiedProj[2][3] = (-2.0f * f * n) / (f - n);*/
		
		glm::mat4 projectionMatrix = glm::perspectiveFov<float>(camera->m_frustum.m_fov, (float)m_engineContext->m_renderer->GetWidth(), (float)m_engineContext->m_renderer->GetHeight(), camera->m_frustum.m_near, 200.0f);

		RootForce::Frustum frustum;
		frustum.RecalculatePlanesEx(camera->m_viewMatrix, projectionMatrix);

		// Cull terrain chunks.
		m_quadTree->m_culledNodes.clear();
		m_quadTree->CullNodes(&frustum, m_quadTree->GetRoot());

		int j = 0;
		for(auto itr = m_quadTree->m_culledNodes.begin(); itr != m_quadTree->m_culledNodes.end(); ++itr)
		{
			AABB bounds = (*itr)->GetBounds();
			glm::vec3 center = bounds.GetCenter();
			float d = glm::distance(transform->m_position, center);

			m_distances[j] = d;
	
			// Render terrain entities.
			for(auto entity = (*itr)->m_terrainEntityIndices.begin(); entity != (*itr)->m_terrainEntityIndices.end(); ++entity)
			{
				RootForce::Renderable* renderable = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(m_quadTree->m_entities[(*entity)]);

				Render::RenderJob job;
				job.m_mesh = m_meshes[j];

				// Set geometry data.
				job.m_mesh->SetVertexBuffer(renderable->m_model->m_meshes[0]->GetVertexBuffer());
				job.m_mesh->SetElementBuffer(renderable->m_model->m_meshes[0]->GetElementBuffer());
				job.m_mesh->SetVertexAttribute(renderable->m_model->m_meshes[0]->GetVertexAttribute());
				
				job.m_mesh->SetPrimitiveType(GL_PATCHES);
				job.m_material = m_material;
	
				// Set lod level.
				job.m_params[Render::Semantic::SPEEDMIN] = &m_distances[j];

				// Assign weight map.
				job.m_material->m_textures[Render::TextureSemantic::TEXTUREMAP] = renderable->m_material->m_textures[Render::TextureSemantic::TEXTUREMAP];

				job.m_flags = Render::RenderFlags::RENDER_IGNORE_CASTSHADOW;

				m_engineContext->m_renderer->AddRenderJob(job);
				j++;
			}
		}
		std::cout << "Culled nodes " << j << std::endl;
	}
}