#include "RootSystems/Include/Frustum.h"

#include "RootEngine/Include/GameSharedContext.h"

extern RootEngine::GameSharedContext g_engineContext;

using glm::vec3;

namespace RootForce
{
	void Frustum::RecalculatePlanes()
	{
		//I'm using the geometrical way of calculating the corners.
		//There is a way to do this with matrix inverses instead, but I never
		//managed to make it work.
		vec3 frontvec = vec3(0.0f, 0.0f, -1.0f);
		vec3 rightvec = vec3(1.0f, 0.0f, 0.0f);
		vec3 upvec = vec3(0.0f, 1.0f, 0.0f);


		float fov = glm::radians(m_fov);

		float halfFarWidth = glm::tan(fov / 2.0f) * m_far * m_aspectRatio;
		float halfFarHeight = (glm::tan(fov / 2.0f) * m_far);
		float halfNearWidth = glm::tan(fov / 2.0f) * m_near * m_aspectRatio;
		float halfNearHeight = (glm::tan(fov / 2.0f) * m_near);

		vec3 farCenter = (frontvec * m_far);
		vec3 nearCenter = (frontvec * m_near);

		ftl = farCenter + (upvec * halfFarHeight) - (rightvec * halfFarWidth);
		ftr = farCenter + (upvec * halfFarHeight) + (rightvec * halfFarWidth);
		fbl = farCenter - (upvec * halfFarHeight) - (rightvec * halfFarWidth);
		fbr = farCenter - (upvec * halfFarHeight) + (rightvec * halfFarWidth);

		ntl = nearCenter + (upvec * halfNearHeight) - (rightvec * halfNearWidth);
		ntr = nearCenter + (upvec * halfNearHeight) + (rightvec * halfNearWidth);
		nbl = nearCenter - (upvec * halfNearHeight) - (rightvec * halfNearWidth);
		nbr = nearCenter - (upvec * halfNearHeight) + (rightvec * halfNearWidth);

		m_planes[NEARP].SetNormalAndPoint(frontvec, nearCenter);
		m_planes[FARP].SetNormalAndPoint(-frontvec, farCenter);

		vec3 aux, normal;

		aux = (nearCenter + upvec * halfNearHeight);
		aux = glm::normalize(aux);
		normal = glm::cross(aux, rightvec);
		m_planes[TOP].SetNormalAndPoint(normal, nearCenter + upvec * halfNearHeight);

		aux = (nearCenter - upvec * halfNearHeight);
		aux = glm::normalize(aux);
		normal = glm::cross(rightvec, aux);
		m_planes[BOTTOM].SetNormalAndPoint(normal, nearCenter - upvec * halfNearHeight);

		aux = (nearCenter - rightvec * halfNearWidth);
		aux = glm::normalize(aux);
		normal = glm::cross(aux, upvec);
		m_planes[LEFT].SetNormalAndPoint(normal, nearCenter - rightvec * halfNearWidth);

		aux = (nearCenter + rightvec * halfNearWidth);
		aux = glm::normalize(aux);
		normal = glm::cross(upvec, aux);
		m_planes[RIGHT].SetNormalAndPoint(normal, nearCenter + rightvec * halfNearWidth);
	}

	void Frustum::DrawLines(glm::mat4 p_space)
	{
		g_engineContext.m_renderer->AddLine(ntl, ntr, glm::vec4(1.0f));
		g_engineContext.m_renderer->AddLine(nbl, nbr, glm::vec4(1.0f));
		g_engineContext.m_renderer->AddLine(ftl, ftr, glm::vec4(1.0f));
		g_engineContext.m_renderer->AddLine(fbl, fbr, glm::vec4(1.0f));
		g_engineContext.m_renderer->AddLine(ntl, nbl, glm::vec4(1.0f));
		g_engineContext.m_renderer->AddLine(ntr, nbr, glm::vec4(1.0f));
		g_engineContext.m_renderer->AddLine(ntl, ntr, glm::vec4(1.0f));
		g_engineContext.m_renderer->AddLine(ntl, ntr, glm::vec4(1.0f));
	}
}