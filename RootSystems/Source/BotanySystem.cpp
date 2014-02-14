#include <RootSystems/Include/BotanySystem.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>

namespace RootForce
{
	void BotanySystem::Initialize()
	{
		// Load effect.
		m_effect = m_engineContext->m_resourceManager->LoadEffect("Botany");

		// Load material.
		m_material = m_engineContext->m_renderer->CreateMaterial("Botany");
		m_material->m_textures[Render::TextureSemantic::DIFFUSE] = m_engineContext->m_resourceManager->LoadTexture("LightGreen2", Render::TextureType::TEXTURE_2D);
		m_material->m_effect = m_effect;

		// Pre-allocate meshes to use.
		
		char data[28*10000];
		memset(&data, 0, 28*10000);
		
		m_meshes.resize(20);
		for(int i = 0; i < 20; i++)
		{
			m_meshes[i] = m_engineContext->m_renderer->CreateMesh();

			m_meshes[i]->SetTransformFeedback();
			m_meshes[i]->BindTransformFeedback();	
			m_meshes[i]->SetVertexBuffer(m_engineContext->m_renderer->CreateBuffer(GL_ARRAY_BUFFER));
			m_meshes[i]->GetVertexBuffer()->BufferData(1, 28 * 10000, data); 
			glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_meshes[i]->GetVertexBuffer()->GetBufferId());

			m_meshes[i]->SetVertexAttribute(m_engineContext->m_renderer->CreateVertexAttributes());
			m_meshes[i]->GetVertexAttribute()->Init(3);
			m_meshes[i]->GetVertexAttribute()->SetVertexAttribPointer(m_meshes[i]->GetVertexBuffer()->GetBufferId(), 0, 3, GL_FLOAT, GL_FALSE, 28, 0);
			m_meshes[i]->GetVertexAttribute()->SetVertexAttribPointer(m_meshes[i]->GetVertexBuffer()->GetBufferId(), 1, 3, GL_FLOAT, GL_FALSE, 28, (char*)0 + 3 * sizeof(float));
			m_meshes[i]->GetVertexAttribute()->SetVertexAttribPointer(m_meshes[i]->GetVertexBuffer()->GetBufferId(), 2, 1, GL_FLOAT, GL_FALSE, 28, (char*)0 + 6 * sizeof(float));
			
		}
	}

	void BotanySystem::SetQuadTree(QuadTree* p_quadTree)
	{
		m_quadTree = p_quadTree;
	}

	void BotanySystem::UpdateGeometry()
	{
		// Get player.
		ECS::Entity* player = m_world->GetTagManager()->GetEntityByTag("Player");
		RootForce::Transform* ptransform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(player);

		// Get eye camera.
		ECS::Entity* entity = m_world->GetTagManager()->GetEntityByTag("Camera");
		RootForce::Camera* camera = m_world->GetEntityManager()->GetComponent<RootForce::Camera>(entity);
		RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(entity);

		// Cull range.
		float farPlane = 50.0f;
		glm::mat4 projectionMatrix = glm::perspectiveFov<float>(camera->m_frustum.m_fov, (float)m_engineContext->m_renderer->GetWidth(),
			(float)m_engineContext->m_renderer->GetHeight(), camera->m_frustum.m_near, farPlane);

		RootForce::Frustum frustum;
		frustum.RecalculatePlanesEx(camera->m_viewMatrix, projectionMatrix);

		// Cull terrain chunks.
		m_quadTree->m_culledNodes.clear();
		m_quadTree->CullNodes(&frustum, m_quadTree->GetRoot());

		glEnable(GL_RASTERIZER_DISCARD); 

		int meshCount = 0;
		for(auto itr = m_quadTree->m_culledNodes.begin(); itr != m_quadTree->m_culledNodes.end(); ++itr)
		{
			// Render terrain entities.
			for(auto entity = (*itr)->m_terrainEntityIndices.begin(); entity != (*itr)->m_terrainEntityIndices.end(); ++entity)
			{
				RootForce::Renderable* renderable = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(m_quadTree->m_entities[(*entity)]);
				renderable->m_material->m_textures[Render::TextureSemantic::TEXTUREMAP]->Bind(6);		
				
				renderable->m_model->m_meshes[0]->Bind();

				// Fetch an empty mesh to output data into.
				Render::MeshInterface* mesh = m_meshes[meshCount];
				mesh->BindTransformFeedback();

				// Apply update program.
				m_effect->GetTechniques()[0]->GetPrograms()[0]->Apply();

				glBeginTransformFeedback(GL_POINTS);
				renderable->m_model->m_meshes[0]->Draw();
				glEndTransformFeedback();

				renderable->m_model->m_meshes[0]->Unbind();
				meshCount++;
			}
		}

		glDisable(GL_RASTERIZER_DISCARD); 
	}

	void BotanySystem::Process()
	{
		/*
		// Get player.
		ECS::Entity* player = m_world->GetTagManager()->GetEntityByTag("Player");
		RootForce::Transform* ptransform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(player);

		// Get eye camera.
		ECS::Entity* entity = m_world->GetTagManager()->GetEntityByTag("Camera");
		RootForce::Camera* camera = m_world->GetEntityManager()->GetComponent<RootForce::Camera>(entity);
		RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(entity);

		// Cull range.
		float farPlane = 50.0f;

		glm::mat4 projectionMatrix = glm::perspectiveFov<float>(camera->m_frustum.m_fov, (float)m_engineContext->m_renderer->GetWidth(),
			(float)m_engineContext->m_renderer->GetHeight(), camera->m_frustum.m_near, farPlane);

		RootForce::Frustum frustum;
		frustum.RecalculatePlanesEx(camera->m_viewMatrix, projectionMatrix);

		// Cull terrain chunks.
		m_quadTree->m_culledNodes.clear();
		m_quadTree->CullNodes(&frustum, m_quadTree->GetRoot());

		int renderJobCount = 0;
		for(auto itr = m_quadTree->m_culledNodes.begin(); itr != m_quadTree->m_culledNodes.end(); ++itr)
		{
			// Render terrain entities.
			for(auto entity = (*itr)->m_terrainEntityIndices.begin(); entity != (*itr)->m_terrainEntityIndices.end(); ++entity)
			{
				RootForce::Renderable* renderable = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(m_quadTree->m_entities[(*entity)]);

				Render::RenderJob job;
				
				// Set geometry data.
				job.m_mesh = m_meshes[renderJobCount];
				job.m_mesh->SetVertexBuffer(renderable->m_model->m_meshes[0]->GetVertexBuffer());
				job.m_mesh->SetElementBuffer(renderable->m_model->m_meshes[0]->GetElementBuffer());
				job.m_mesh->SetVertexAttribute(renderable->m_model->m_meshes[0]->GetVertexAttribute());
	
				// TODO: Change to effect buffer.
				job.m_params[Render::Semantic::POSITION] = &transform->m_position;

				// Assign weight map.
				job.m_material = m_material;
				job.m_material->m_textures[Render::TextureSemantic::TEXTUREMAP] = renderable->m_material->m_textures[Render::TextureSemantic::TEXTUREMAP];

				// Don't render terrain chunks to shadow map.
				job.m_flags = Render::RenderFlags::RENDER_IGNORE_CASTSHADOW;

				m_engineContext->m_renderer->AddRenderJob(job);
				renderJobCount++;
			}
		}
		*/
	}
}