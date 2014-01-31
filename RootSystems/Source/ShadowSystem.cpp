#include <RootSystems/Include/ShadowSystem.h>

#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/Camera.h>
#include <glm/gtc/swizzle.hpp>

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

		glm::mat4 lightSpace = glm::inverse(tempWorldMatrix);

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
			glm::vec4 cornerInLightSpace = lightSpace * glm::vec4(worldCorners[i], 1.0f);
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

		ECS::Entity* cameraEntity = m_world->GetTagManager()->GetEntityByTag("Camera");
		RootForce::Camera* camera = m_world->GetEntityManager()->GetComponent<RootForce::Camera>(cameraEntity);
		Frustum frustum = camera->m_frustum;
		//frustum.DrawLines(glm::mat4(1.0f), g_engineContext.m_renderer);
		glm::vec4 frustumCorners[8];
		frustumCorners[0] = glm::vec4(frustum.ntl, 1.0f);
		frustumCorners[1] = glm::vec4(frustum.ntr, 1.0f);
		frustumCorners[2] = glm::vec4(frustum.nbl, 1.0f);
		frustumCorners[3] = glm::vec4(frustum.nbr, 1.0f);
		frustumCorners[4] = glm::vec4(frustum.ftl, 1.0f);
		frustumCorners[5] = glm::vec4(frustum.ftr, 1.0f);
		frustumCorners[6] = glm::vec4(frustum.fbl, 1.0f);
		frustumCorners[7] = glm::vec4(frustum.fbr, 1.0f);
		for(int i = 0; i < 8; i++)
		{
			frustumCorners[i] = camera->m_viewMatrix * frustumCorners[i];
		}
		glm::vec3 directions[4];
		for(int i = 0; i < 4; i++)
		{
			directions[i].x = glm::normalize(frustumCorners[i+4].x - frustumCorners[i].x);
			directions[i].y = glm::normalize(frustumCorners[i+4].y - frustumCorners[i].y);
			directions[i].z = glm::normalize(frustumCorners[i+4].z - frustumCorners[i].z);
		}
		float _near[RENDER_SHADOW_CASCADES];
		_near[0] = camera->m_frustum.m_near;
		_near[1] = 15.0f;
		_near[2] = 60.0f;
		_near[3] = 200.0f;
		float _far[RENDER_SHADOW_CASCADES];
		_far[0] = _near[1];
		_far[1] = _near[2];
		_far[2] = _near[3];
		_far[3] = camera->m_frustum.m_far;
		for(int i = 0; i < RENDER_SHADOW_CASCADES; i++)
		{
			AABB boundingbox;
			for(int p = 0; p < 4; p++)
			{
				glm::vec3 nearCorner;
				nearCorner = glm::swizzle<glm::X, glm::Y, glm::Z>(frustumCorners[p]);
				boundingbox.Expand(nearCorner + directions[p] * _near[i]);
				boundingbox.Expand(nearCorner + directions[p] * _far[i]);
			}
			boundingbox.DebugDraw(g_engineContext.m_renderer, glm::vec3(1.0f, 1.0f, 0.0f), glm::inverse(camera->m_viewMatrix));
			glm::vec3 center = boundingbox.GetCenter();
			glm::vec3 centerInWorldSpace = glm::swizzle<glm::X, glm::Y, glm::Z>(glm::inverse(camera->m_viewMatrix) * glm::vec4(center, 1.0f)); 
			glm::vec4 centerInViewSpace = lightSpace * glm::vec4(centerInWorldSpace, 1.0f);
			float nearPlane = 1.0f;
			float lookAtDistance = glm::length(centerInViewSpace - m_maxWorldZ) + nearPlane;
			float radius = glm::length(center - glm::vec3(boundingbox.m_maxX, boundingbox.m_maxY, boundingbox.m_maxZ)); 
			float farPlane = lookAtDistance + radius;
			sc.m_projectionMatrices[i] = glm::ortho(-radius, radius, -radius, radius, nearPlane, farPlane);
			sc.m_viewMatrices[i] = glm::lookAt(centerInWorldSpace + tOr.GetFront() * lookAtDistance, centerInWorldSpace - tOr.GetFront() * lookAtDistance, tOr.GetUp());
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