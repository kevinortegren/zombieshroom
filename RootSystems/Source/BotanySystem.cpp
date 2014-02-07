#include <RootSystems/Include/BotanySystem.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>

namespace RootForce
{
	void BotanySystem::Initialize()
	{
		m_effect = m_engineContext->m_resourceManager->LoadEffect("Botany");

		// Calculate AABB of painted entities.
		CalculatePaintedAABB();

		m_view = glm::lookAt(glm::vec3(0,1,0), glm::vec3(0,0,0), glm::vec3(0,0,1));

		glm::vec4 min = glm::vec4(m_aabb.m_minX, m_aabb.m_minY, m_aabb.m_minZ, 1.0f);
		glm::vec4 max = glm::vec4(m_aabb.m_maxX, m_aabb.m_maxY, m_aabb.m_maxZ, 1.0f);

		m_proj = glm::ortho(min.x, max.x, min.z, max.z, -max.y, -min.y);
	}

	void BotanySystem::CalculatePaintedAABB()
	{
		// Get working set.
		ECS::GroupManager::GroupRange range = m_world->GetGroupManager()->GetEntitiesInGroup("Painted");

		std::vector<glm::vec3> vertices;
		for(auto itr = range.first; itr != range.second; ++itr)
		{
			ECS::Entity* entity = (*itr).second;

			RootForce::Renderable* renderable = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(entity);
			auto material = renderable->m_material;
			auto mesh = renderable->m_model->m_meshes[0];

			RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(entity);
			glm::mat4x4 transformMatrix;
			transformMatrix = glm::translate(glm::mat4(1.0f), transform->m_position);
			transformMatrix = glm::rotate(transformMatrix, transform->m_orientation.GetAngle(), transform->m_orientation.GetAxis());
			transformMatrix = glm::scale(transformMatrix, transform->m_scale);

			// Parse vertex data.
			glBindBuffer(GL_ARRAY_BUFFER, mesh->GetVertexBuffer()->GetBufferId());
			unsigned char* data = (unsigned char*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);

			int offset = 0;
			for(unsigned i = 0; i < mesh->GetVertexBuffer()->GetBufferSize(); i += mesh->GetVertexBuffer()->GetElementSize())
			{
				Render::Vertex1P1N1UV1T1BT v;
				v.m_tangent = glm::vec3(0);
				v.m_bitangent = glm::vec3(0);
				memcpy(&v, &data[i], mesh->GetVertexBuffer()->GetElementSize());

				// Transform vertex to world space.
				glm::vec4 tf = transformMatrix * glm::vec4(v.m_pos, 1.0f);
				v.m_pos = glm::vec3(tf.x, tf.y, tf.z);

				vertices.push_back(v.m_pos);
				offset++;
			}

			glUnmapBuffer(GL_ARRAY_BUFFER);
		}

		float maxX, maxY, maxZ, minX, minY, minZ;
		maxX = maxY = maxZ = -99999;
		minX = minY = minZ = 99999;

		for(auto itr = vertices.begin(); itr != vertices.end(); ++itr)
		{
			glm::vec3 pos = (*itr);
			for(int i = 0; i < 3; i++)
			{
				float tx = pos.x;
				float ty = pos.y;
				float tz = pos.z;

				if(tx > maxX) maxX = (int)tx;
				if(tx < minX) minX = (int)tx;
				
				if(ty > maxY) maxY = (int)ty;
				if(ty < minY) minY = (int)ty;

				if(tz > maxZ) maxZ = (int)tz;
				if(tz < minZ) minZ = (int)tz;	
			}
		}

		m_aabb.m_maxX = maxX;
		m_aabb.m_minX = minX;
		m_aabb.m_maxY = maxY;
		m_aabb.m_minY = minY;
		m_aabb.m_maxZ = maxZ;
		m_aabb.m_minZ = minZ;

		m_width = m_aabb.m_maxX - m_aabb.m_minX;
		m_height = m_aabb.m_maxZ - m_aabb.m_minZ;
	}

	void BotanySystem::DensityRenderToTexture(RootForce::RenderingSystem* m_renderingSystem)
	{
		// Create RTT.
		m_density = m_engineContext->m_renderer->CreateRenderToTexture();
		m_density->SetTexture(m_engineContext->m_renderer->CreateTexture());
		m_density ->GetTexture()->CreateEmptyTexture(1024, 1024, Render::TextureFormat::TEXTURE_RGB);
		m_density->BindTexture();

		// Set Camera.
		m_engineContext->m_renderer->SetViewMatrix(m_view);
		m_engineContext->m_renderer->SetProjectionMatrix(m_proj);

		// Draw painted geometry.
		ECS::GroupManager::GroupRange range = m_world->GetGroupManager()->GetEntitiesInGroup("Painted");
		for(auto itr = range.first; itr != range.second; ++itr)
		{
			ECS::Entity* entity = (*itr).second;

			RootForce::Renderable* renderable = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(entity);
			auto material = renderable->m_material;
			auto mesh = renderable->m_model->m_meshes[0];

			RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(entity);

			// Calculate model/normal.
			m_matrices[entity].m_model = glm::translate(glm::mat4(1.0f), transform->m_position);
			m_matrices[entity].m_model = glm::rotate(m_matrices[entity].m_model, transform->m_orientation.GetAngle(), transform->m_orientation.GetAxis());
			m_matrices[entity].m_model = glm::scale(m_matrices[entity].m_model, transform->m_scale);
			m_matrices[entity].m_normal = glm::mat4(glm::transpose(glm::inverse(glm::mat3(m_matrices[entity].m_model))));
			m_matrices[entity].m_model = m_matrices[entity].m_model;

			Render::RenderJob job;
			job.m_flags = renderable->m_renderFlags;
			job.m_mesh = renderable->m_model->m_meshes[0];
			job.m_shadowMesh = renderable->m_model->m_meshes[0];
			job.m_params = renderable->m_params;
			job.m_params[Render::Semantic::MODEL] = &m_matrices[entity].m_model;
			job.m_params[Render::Semantic::NORMAL] = &m_matrices[entity].m_normal;
			job.m_material = renderable->m_material;
			job.m_renderPass = renderable->m_pass;

			m_engineContext->m_renderer->AddRenderJob(job);
		}

		m_engineContext->m_renderer->SetRenderToTexture(m_density);
		m_engineContext->m_renderer->Render();

		// Store texture.
		m_density->Store("rtt.tga");


		// Restore backbuffer.
		m_engineContext->m_renderer->SetRenderToTexture(nullptr);

		m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(m_world->GetTagManager()->GetEntityByTag("Player"))->m_material->m_textures[Render::TextureSemantic::DIFFUSE] = m_density->GetTexture();
	}

	void BotanySystem::Process()
	{
		m_aabb.DebugDraw(m_engineContext->m_renderer, glm::vec3(1,1,0), glm::mat4(1.0f));
	}
}