#include <RootSystems/Include/BotanySystem.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>

namespace RootForce
{
	void BotanySystem::Divide()
	{
		// Divide painted meshes into chunks.
		m_quadTree.BeginDivide(BOTANY_VERTICES_PER_TERRAIN_CHUNK, true, false);
	}

	void BotanySystem::Initialize(BotanyTextures& m_textures)
	{
		// Load effect.
		m_effect = m_engineContext->m_resourceManager->LoadEffect("Botany");
		m_effect->GetTechniques()[1]->m_perTechniqueBuffer->BufferData(1, sizeof(glm::vec3), &glm::vec3(0));

		// Load material.
		m_material = m_engineContext->m_renderer->CreateMaterial("Botany");
		
		// Diffuse texture for geometric grass blades.
		// R
		m_material->m_textures[Render::TextureSemantic::DIFFUSE] = m_engineContext->m_resourceManager->LoadTexture(m_textures.m_diffuse, Render::TextureType::TEXTURE_2D);
		
		// Transluency texture for geometric grass blades.
		// R
		m_material->m_textures[Render::TextureSemantic::TRANSLUCENCY] = m_engineContext->m_resourceManager->LoadTexture(m_textures.m_translucency, Render::TextureType::TEXTURE_2D);

		// Imposter diffuse.
		m_material->m_textures[Render::TextureSemantic::DIFFUSE1] = m_engineContext->m_resourceManager->LoadTexture(m_textures.m_billboard, Render::TextureType::TEXTURE_2D);

		// Terrain grass texture.
		m_material->m_textures[Render::TextureSemantic::DIFFUSE2] = m_engineContext->m_resourceManager->LoadTexture(m_textures.m_terrainTexture, Render::TextureType::TEXTURE_2D);
		m_material->m_effect = m_effect;
	
		m_quadTree.Initialize(m_engineContext, m_world, "Painted");
		Divide();

		// Allocate mesh memory.
		char* data = new char[BOTANY_CELL_SIZE];
		memset(data, 0, BOTANY_CELL_SIZE);
		for(int i = 0; i < BOTANY_MESHES_SIZE; i++)
		{
			m_meshes[i] = m_engineContext->m_renderer->CreateMesh();

			m_meshes[i]->SetTransformFeedback();
			m_meshes[i]->BindTransformFeedback();	
			m_meshes[i]->SetVertexBuffer(m_engineContext->m_renderer->CreateBuffer(GL_ARRAY_BUFFER));
			m_meshes[i]->GetVertexBuffer()->BufferData(1, BOTANY_CELL_SIZE, data); 
			glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_meshes[i]->GetVertexBuffer()->GetBufferId());

			m_meshes[i]->SetVertexAttribute(m_engineContext->m_renderer->CreateVertexAttributes());
			m_meshes[i]->GetVertexAttribute()->Init(3);
			m_meshes[i]->GetVertexAttribute()->SetVertexAttribPointer(m_meshes[i]->GetVertexBuffer()->GetBufferId(), 0, 3, GL_FLOAT, GL_FALSE, 28, 0);
			m_meshes[i]->GetVertexAttribute()->SetVertexAttribPointer(m_meshes[i]->GetVertexBuffer()->GetBufferId(), 1, 3, GL_FLOAT, GL_FALSE, 28, (char*)0 + 3 * sizeof(float));
			m_meshes[i]->GetVertexAttribute()->SetVertexAttribPointer(m_meshes[i]->GetVertexBuffer()->GetBufferId(), 2, 1, GL_FLOAT, GL_FALSE, 28, (char*)0 + 6 * sizeof(float));

			m_meshes[i]->SetPrimitiveType(GL_POINTS);			
		}

		delete[] data;
		m_meshCount = 0;

		for(int i = 0; i < BOTANY_MESHES_SIZE; i++)
		{
			m_cellDirectory[i] = 0;
		}

		m_initialized = true;
	}

	void BotanySystem::Construct(QuadNode* p_node)
	{
		glEnable(GL_RASTERIZER_DISCARD); 
		for(auto entity = p_node->m_indices.begin(); entity != p_node->m_indices.end(); ++entity)
		{
			// Reset mesh counter to resuse old meshes.
			if(m_meshCount >= BOTANY_MESHES_SIZE)
			{
				m_meshCount = 0;
			}

			RootForce::Renderable* renderable = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(m_quadTree.m_entities[(*entity)]);
			renderable->m_material->m_textures[Render::TextureSemantic::TEXTUREMAP]->Bind(6);				
			renderable->m_model->m_meshes[0]->Bind();

			// Fetch an empty mesh to output data into.
			Render::MeshInterface* mesh = m_meshes[m_meshCount];

			// If a cell already uses this mesh, remove it.
			if(m_cells[m_cellDirectory[m_meshCount]].m_meshSize != 0)
			{
				for(int i = 0; i < m_cells[m_cellDirectory[m_meshCount]].m_meshSize; i++)
				{
					m_cells[m_cellDirectory[m_meshCount]].m_meshIndices[i] = -1;
				}
				m_cells[m_cellDirectory[m_meshCount]].m_meshSize = 0;
			}

			// Store which cell this mesh belong to.
			m_cellDirectory[m_meshCount] = p_node->m_id;

			mesh->BindTransformFeedback();

			// Apply update program.
			m_effect->GetTechniques()[0]->GetPrograms()[0]->Apply();

			renderable->m_model->m_meshes[0]->SetPrimitiveType(GL_PATCHES);

			glBeginTransformFeedback(GL_POINTS);
			renderable->m_model->m_meshes[0]->Draw();
			glEndTransformFeedback();

			renderable->m_model->m_meshes[0]->SetPrimitiveType(GL_TRIANGLES);
			renderable->m_model->m_meshes[0]->Unbind();

			m_cells[p_node->m_id].m_meshIndices[m_cells[p_node->m_id].m_meshSize] = m_meshCount;
			m_cells[p_node->m_id].m_meshSize++;

			m_meshCount++;
		}
		glDisable(GL_RASTERIZER_DISCARD); 
	}


	void BotanySystem::Process()
	{
		if(m_initialized)
		{
			// Get player position.
			ECS::Entity* player = m_world->GetTagManager()->GetEntityByTag("Player");
			RootForce::Transform* ptransform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(player);

			// Buffer player position.
			m_effect->GetTechniques()[1]->m_perTechniqueBuffer->BufferSubData(0, sizeof(glm::vec3), &ptransform->m_position);

			// Get eye camera.
			ECS::Entity* entity = m_world->GetTagManager()->GetEntityByTag("Camera");
			RootForce::Camera* camera = m_world->GetEntityManager()->GetComponent<RootForce::Camera>(entity);
			RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(entity);

			float farPlane = BOTANY_CULL_RANGE;
			glm::mat4 projectionMatrix = glm::perspectiveFov<float>(camera->m_frustum.m_fov, (float)m_engineContext->m_renderer->GetWidth(),
				(float)m_engineContext->m_renderer->GetHeight(), camera->m_frustum.m_near, farPlane);

			RootForce::Frustum frustum;
			frustum.RecalculatePlanesEx(camera->m_viewMatrix, projectionMatrix);

			// Cull nodes.
			m_quadTree.m_culledNodes.clear();
			m_quadTree.Cull(&frustum, m_quadTree.GetRoot());

			for(auto itr = m_quadTree.m_culledNodes.begin(); itr != m_quadTree.m_culledNodes.end(); ++itr)
			{			
				int id = (*itr)->m_id;
				assert(id < BOTANY_MAX_CELLS && "Botany terrain chunk id exceeds max botany cells.");

				// Bug in quad tree culling, returning nodes with id -1.
				if(id == -1)
					continue;

				// If mesh is not constructed.
				if(m_cells[id].m_meshSize == 0)
				{
					Construct((*itr));
				}

				// Render result.
				for(int j = 0; j < m_cells[id].m_meshSize; j++)
				{
					assert(m_cells[id].m_meshIndices[j] != -1 && "Attempting to render botany cell with mesh index -1.");

					Render::RenderJob job;
					job.m_mesh = m_meshes[m_cells[id].m_meshIndices[j]];
					job.m_mesh->SetNoCulling(true);
					job.m_material = m_material;
					job.m_flags = Render::RenderFlags::RENDER_IGNORE_CASTSHADOW;
					m_engineContext->m_renderer->AddRenderJob(job);
				}
			}
		}
	}
}