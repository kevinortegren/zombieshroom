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

		float maxWorldX = -99999;
		float minWorldX = 99999;
		float maxWorldY = -99999;
		float minWorldY = 99999;
		float maxWorldZ = -99999;
		float minWorldZ = 99999;
		for(int i = 0; i < 8; i++)
		{
			glm::vec4 cornerInLightSpace = sc.m_viewMatrix * glm::vec4(worldCorners[i], 1.0f);
			if(cornerInLightSpace.x < minWorldX)
			{
				minWorldX = cornerInLightSpace.x;
			}
			if(cornerInLightSpace.x > maxWorldX)
			{
				maxWorldX = cornerInLightSpace.x;
			}
			if(cornerInLightSpace.y < minWorldY)
			{
				minWorldY = cornerInLightSpace.y;
			}
			if(cornerInLightSpace.y > maxWorldY)
			{
				maxWorldY = cornerInLightSpace.y;
			}
			if(cornerInLightSpace.z < minWorldZ)
			{
				minWorldZ = cornerInLightSpace.z;
			}
			if(cornerInLightSpace.z > maxWorldZ)
			{
				maxWorldZ = cornerInLightSpace.z;
			}
		}

		ECS::Entity* cameraEntity = m_world->GetTagManager()->GetEntityByTag("Camera");
		RootForce::Camera* camera = m_world->GetEntityManager()->GetComponent<RootForce::Camera>(cameraEntity);
		Frustum frustum = camera->m_frustum;
		glm::mat4 invViewMatrix = glm::inverse(camera->m_viewMatrix);
		glm::vec4 frustumCorners[8];
		frustumCorners[0] = glm::vec4(frustum.ntl, 1.0f);
		frustumCorners[1] = glm::vec4(frustum.nbl, 1.0f);
		frustumCorners[3] = glm::vec4(frustum.nbr, 1.0f);
		frustumCorners[4] = glm::vec4(frustum.ftl, 1.0f);
		frustumCorners[5] = glm::vec4(frustum.ftr, 1.0f);
		frustumCorners[6] = glm::vec4(frustum.fbl, 1.0f);
		frustumCorners[7] = glm::vec4(frustum.fbr, 1.0f);

		float maxFrustumX = -99999;
		float minFrustumX = 99999;
		float maxFrustumY = -99999;
		float minFrustumY = 99999;
		float maxFrustumZ = -99999;
		float minFrustumZ = 99999;
		for(int i = 0; i < 8; i++)
		{
			glm::vec4 cornerInLightSpace = sc.m_viewMatrix * frustumCorners[i];
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
		float minX = glm::max(minWorldX, minFrustumX);
		float maxX = glm::min(maxWorldX, maxFrustumX);
		float minY = glm::max(minWorldY, minFrustumY);
		float maxY = glm::min(maxWorldY, maxFrustumY);
		float minZ = glm::max(minWorldZ, minFrustumZ);
		float maxZ = maxWorldZ;

		sc.m_projectionMatrix = glm::ortho(minX, maxX, minY, maxY, -maxZ, -minZ);

		g_engineContext.m_renderer->AddShadowcaster(sc, m_shadowcasterCount);

		m_shadowcasterCount++;
	}

	void ShadowSystem::End()
	{

	}
}