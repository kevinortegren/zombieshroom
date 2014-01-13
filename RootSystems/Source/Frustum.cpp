#include "RootSystems/Include/Frustum.h"

using glm::vec3;

namespace RootForce
{
	void Frustum::recalculatePlanes()
	{
		//I'm using the geometrical way of calculating the corners.
		//There is a way to do this with matrix inverses instead, but I never
		//managed to make it work.
		vec3 frontvec = vec3(0.0f, 0.0f, -1.0f);
		vec3 rightvec = vec3(1.0f, 0.0f, 0.0f);
		vec3 upvec = vec3(0.0f, 1.0f, 0.0f);


		float fov = radians(m_fov);

		float halfFarWidth = glm::tan(fov / 2.0f) * m_far * m_aspectRatio;
		float halfFarHeight = (glm::tan(fov / 2.0f) * m_far);
		float halfNearWidth = glm::tan(fov / 2.0f) * m_near * m_aspectRatio;
		float halfNearHeight = (glm::tan(fov / 2.0f) * m_near);

		vec3 farCenter = (frontvec * m_far);
		vec3 nearCenter = (frontvec * m_near);

		vec3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
		ftl = farCenter + (upvec * halfFarHeight) - (rightvec * halfFarWidth);
		ftr = farCenter + (upvec * halfFarHeight) + (rightvec * halfFarWidth);
		fbl = farCenter - (upvec * halfFarHeight) - (rightvec * halfFarWidth);
		fbr = farCenter - (upvec * halfFarHeight) + (rightvec * halfFarWidth);

		ntl = nearCenter + (upvec * halfNearHeight) - (rightvec * halfNearWidth);
		ntr = nearCenter + (upvec * halfNearHeight) + (rightvec * halfNearWidth);
		nbl = nearCenter - (upvec * halfNearHeight) - (rightvec * halfNearWidth);
		nbr = nearCenter - (upvec * halfNearHeight) + (rightvec * halfNearWidth);

		m_planes[NEARP].setNormalAndPoint(frontvec, nearCenter);
		m_planes[FARP].setNormalAndPoint(-frontvec, farCenter);

		vec3 aux, normal;

		aux = (nearCenter + upvec * halfNearHeight);
		aux = normalize(aux);
		normal = cross(aux, rightvec);
		m_planes[TOP].setNormalAndPoint(normal, nearCenter + upvec * halfNearHeight);

		aux = (nearCenter - upvec * halfNearHeight);
		aux = normalize(aux);
		normal = cross(rightvec, aux);
		m_planes[BOTTOM].setNormalAndPoint(normal, nearCenter - upvec * halfNearHeight);

		aux = (nearCenter - rightvec * halfNearWidth);
		aux = normalize(aux);
		normal = cross(aux, upvec);
		m_planes[LEFT].setNormalAndPoint(normal, nearCenter - rightvec * halfNearWidth);

		aux = (nearCenter + rightvec * halfNearWidth);
		aux = normalize(aux);
		normal = cross(upvec, aux);
		m_planes[RIGHT].setNormalAndPoint(normal, nearCenter + rightvec * halfNearWidth);
	}
}