#include <RootSystems/Include/ShadowSystem.h>

#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/Camera.h>

extern RootEngine::GameSharedContext g_engineContext;

namespace RootForce
{
	void ShadowSystem::Init()
	{
		m_shadowcasters.Init(m_world->GetEntityManager());
		m_transforms.Init(m_world->GetEntityManager());

		m_shadowcasterCount = 0;
	}

	void ShadowSystem::Begin()
	{
		m_shadowcasterCount = 0;
	}

	void ShadowSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		Transform* transform = m_transforms.Get(p_entity);
		Shadowcaster* shadowcaster = m_shadowcasters.Get(p_entity);

		Render::Shadowcaster sc;
		
		Orientation tOr = transform->m_orientation;
		tOr.Yaw(180.0f);
		glm::mat4 tempWorldMatrix;
		tempWorldMatrix = glm::translate(glm::mat4(1.0f), transform->m_position);
		tempWorldMatrix = glm::rotate(tempWorldMatrix, tOr.GetAngle(), tOr.GetAxis());
		tempWorldMatrix = glm::scale(tempWorldMatrix, transform->m_scale);

		sc.m_viewMatrix = glm::inverse(tempWorldMatrix);

		glm::vec3 worldCorners[8];
		worldCorners[0] = glm::vec3(m_worldAABB.m_maxX, m_worldAABB.m_maxY, m_worldAABB.m_maxZ);
		worldCorners[1] = glm::vec3(m_worldAABB.m_maxX, m_worldAABB.m_minY, m_worldAABB.m_maxZ);
		worldCorners[2] = glm::vec3(m_worldAABB.m_maxX, m_worldAABB.m_maxY, m_worldAABB.m_minZ);
		worldCorners[3] = glm::vec3(m_worldAABB.m_maxX, m_worldAABB.m_minY, m_worldAABB.m_minZ);
		worldCorners[4] = glm::vec3(m_worldAABB.m_minX, m_worldAABB.m_maxY, m_worldAABB.m_maxZ);
		worldCorners[5] = glm::vec3(m_worldAABB.m_minX, m_worldAABB.m_minY, m_worldAABB.m_maxZ);
		worldCorners[6] = glm::vec3(m_worldAABB.m_minX, m_worldAABB.m_maxY, m_worldAABB.m_minZ);
		worldCorners[7] = glm::vec3(m_worldAABB.m_minX, m_worldAABB.m_minY, m_worldAABB.m_minZ);

		m_maxWorldX = -99999;
		m_minWorldX = 99999;
		m_maxWorldY = -99999;
		m_minWorldY = 99999;
		m_maxWorldZ = -99999;
		m_minWorldZ = 99999;
		for(int i = 0; i < 8; i++)
		{
			glm::vec4 cornerInLightSpace = sc.m_viewMatrix * glm::vec4(worldCorners[i], 1.0f);
			if(cornerInLightSpace.x < m_minWorldX)
			{
				m_minWorldX = cornerInLightSpace.x;
			}
			if(cornerInLightSpace.x > m_maxWorldX)
			{
				m_maxWorldX = cornerInLightSpace.x;
			}
			if(cornerInLightSpace.y < m_minWorldY)
			{
				m_minWorldY = cornerInLightSpace.y;
			}
			if(cornerInLightSpace.y > m_maxWorldY)
			{
				m_maxWorldY = cornerInLightSpace.y;
			}
			if(cornerInLightSpace.z < m_minWorldZ)
			{
				m_minWorldZ = cornerInLightSpace.z;
			}
			if(cornerInLightSpace.z > m_maxWorldZ)
			{
				m_maxWorldZ = cornerInLightSpace.z;
			}
		}

		ECS::Entity* cameraEntity = m_world->GetTagManager()->GetEntityByTag("TestCamera");
		RootForce::Camera* camera = m_world->GetEntityManager()->GetComponent<RootForce::Camera>(cameraEntity);
		Frustum subFrusta[RENDER_SHADOW_CASCADES];
		subFrusta[0] = camera->m_frustum;
		subFrusta[0].m_far = 20.0f;
		subFrusta[0].RecalculatePlanes();
		subFrusta[1] = camera->m_frustum;
		subFrusta[1].m_near = 20.0f;
		subFrusta[1].m_far = 100.0f;
		subFrusta[1].RecalculatePlanes();
		subFrusta[2] = camera->m_frustum;
		subFrusta[2].m_near = 100.0f;
		subFrusta[2].RecalculatePlanes();

		for(int i = 0; i < RENDER_SHADOW_CASCADES; i++)
		{
			//Set cascade ortho matrix
			sc.m_projectionMatrices[i] = OrthoProjectionFromFrustum(&subFrusta[i], sc.m_viewMatrix);
			subFrusta[i].DrawLines(glm::mat4(1.0f), g_engineContext.m_renderer);
		}


		g_engineContext.m_renderer->AddShadowcaster(sc, m_shadowcasterCount);

		m_shadowcasterCount++;
	}

	void ShadowSystem::End()
	{
	}

	glm::mat4 ShadowSystem::OrthoProjectionFromFrustum(Frustum* p_frustum, glm::mat4 p_lightViewMatrix)
	{
		glm::vec4 frustumCorners[8];
		frustumCorners[0] = glm::vec4(p_frustum->ntl, 1.0f);
		frustumCorners[1] = glm::vec4(p_frustum->nbl, 1.0f);
		frustumCorners[3] = glm::vec4(p_frustum->nbr, 1.0f);
		frustumCorners[4] = glm::vec4(p_frustum->ftl, 1.0f);
		frustumCorners[5] = glm::vec4(p_frustum->ftr, 1.0f);
		frustumCorners[6] = glm::vec4(p_frustum->fbl, 1.0f);
		frustumCorners[7] = glm::vec4(p_frustum->fbr, 1.0f);

		float maxFrustumX = -99999;
		float minFrustumX = 99999;
		float maxFrustumY = -99999;
		float minFrustumY = 99999;
		float maxFrustumZ = -99999;
		float minFrustumZ = 99999;
		for(int i = 0; i < 8; i++)
		{
			glm::vec4 cornerInLightSpace = p_lightViewMatrix * frustumCorners[i];
			if(cornerInLightSpace.x < minFrustumX)
			{
				minFrustumX = cornerInLightSpace.x;
			}
			if(cornerInLightSpace.x > maxFrustumX)
			{
				maxFrustumX = cornerInLightSpace.x;
			}
			if(cornerInLightSpace.y < minFrustumY)
			{
				minFrustumY = cornerInLightSpace.y;
			}
			if(cornerInLightSpace.y > maxFrustumY)
			{
				maxFrustumY = cornerInLightSpace.y;
			}
			if(cornerInLightSpace.z < minFrustumZ)
			{
				minFrustumZ = cornerInLightSpace.z;
			}
			if(cornerInLightSpace.z > maxFrustumZ)
			{
				maxFrustumZ = cornerInLightSpace.z;
			}
		}
		float minX = glm::max(m_minWorldX, minFrustumX);
		float maxX = glm::min(m_maxWorldX, maxFrustumX);
		float minY = glm::max(m_minWorldY, minFrustumY);
		float maxY = glm::min(m_maxWorldY, maxFrustumY);
		float minZ = glm::max(m_minWorldZ, minFrustumZ);
		float maxZ = m_maxWorldZ;

		return glm::ortho(minX, maxX, minY, maxY, -maxZ, -minZ);
	}
}