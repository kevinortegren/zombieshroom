#include <RootSystems/Include/Frustum.h>
#include <glm/glm.hpp>

using glm::vec3;

namespace RootForce
{
	void Frustum::RecalculatePlanes()
	{
		//I'm using the geometrical way of calculating the corners.
		//There is a way to do this with matrix inverses instead, but I never
		//managed to make it work.
		vec3 frontvec = vec3(0.0f, 0.0f, 1.0f);
		vec3 rightvec = vec3(-1.0f, 0.0f, 0.0f);
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

	void Frustum::RecalculatePlanesEx(glm::mat4x4& p_view, glm::mat4x4& p_proj)
	{
		glm::mat4 matrix;

		/*float zMinimum = -p_proj[3][2] / p_proj[2][2];
		float r = m_far / (m_far / zMinimum);

		p_proj[2][2] = r;
		p_proj[3][2] = -r;*/

		matrix = p_proj * p_view;

		m_planesEx[NEARP].a = matrix[0][3] + matrix[0][2];
		m_planesEx[NEARP].b = matrix[1][3] + matrix[1][2];
		m_planesEx[NEARP].c = matrix[2][3] + matrix[2][2];
		m_planesEx[NEARP].d = matrix[3][3] + matrix[3][2];

		m_planesEx[FARP].a = matrix[0][3] + matrix[0][2];
		m_planesEx[FARP].b = matrix[1][3] + matrix[1][2];
		m_planesEx[FARP].c = matrix[2][3] + matrix[2][2];
		m_planesEx[FARP].d = matrix[3][3] + matrix[3][2];

		m_planesEx[LEFT].a = matrix[0][3] + matrix[0][0];
		m_planesEx[LEFT].b = matrix[1][3] + matrix[1][0];
		m_planesEx[LEFT].c = matrix[2][3] + matrix[2][0];
		m_planesEx[LEFT].d = matrix[3][3] + matrix[3][0];

		m_planesEx[RIGHT].a = matrix[0][3] + matrix[0][0];
		m_planesEx[RIGHT].b = matrix[1][3] + matrix[1][0];
		m_planesEx[RIGHT].c = matrix[2][3] + matrix[2][0];
		m_planesEx[RIGHT].d = matrix[3][3] + matrix[3][0];

		m_planesEx[TOP].a = matrix[0][3] + matrix[0][1];
		m_planesEx[TOP].b = matrix[1][3] + matrix[1][1];
		m_planesEx[TOP].c = matrix[2][3] + matrix[2][1];
		m_planesEx[TOP].d = matrix[3][3] + matrix[3][1];

		m_planesEx[BOTTOM].a = matrix[0][3] + matrix[0][1];
		m_planesEx[BOTTOM].b = matrix[1][3] + matrix[1][1];
		m_planesEx[BOTTOM].c = matrix[2][3] + matrix[2][1];
		m_planesEx[BOTTOM].d = matrix[3][3] + matrix[3][1];

		//TODO: Calculate corners.

	}

	void Frustum::DrawLines(glm::mat4 p_space, Render::RendererInterface* p_renderer)
	{
		glm::vec4 tntl = p_space * glm::vec4(ntl.x, ntl.y, ntl.z, 1.0f);
		glm::vec4 tnbl = p_space * glm::vec4(nbl.x, nbl.y, nbl.z, 1.0f);
		glm::vec4 tntr = p_space * glm::vec4(ntr.x, ntr.y, ntr.z, 1.0f);
		glm::vec4 tnbr = p_space * glm::vec4(nbr.x, nbr.y, nbr.z, 1.0f);

		glm::vec4 tftl = p_space * glm::vec4(ftl * 0.001f, 1.0f);
		glm::vec4 tfbl = p_space * glm::vec4(fbl * 0.001f, 1.0f);
		glm::vec4 tftr = p_space * glm::vec4(ftr * 0.001f, 1.0f);
		glm::vec4 tfbr = p_space * glm::vec4(fbr * 0.001f, 1.0f);

		p_renderer->AddLine(glm::vec3(tntl), glm::vec3(tntr), glm::vec4(1,1,0,1));
		p_renderer->AddLine(glm::vec3(tnbl), glm::vec3(tnbr), glm::vec4(1,1,0,1));
		p_renderer->AddLine(glm::vec3(tftl), glm::vec3(tftr), glm::vec4(0,1,0,1));
		p_renderer->AddLine(glm::vec3(tfbl), glm::vec3(tfbr), glm::vec4(0,1,0,1));
		p_renderer->AddLine(glm::vec3(tntl), glm::vec3(tnbl), glm::vec4(1,1,0,1));
		p_renderer->AddLine(glm::vec3(tntr), glm::vec3(tnbr), glm::vec4(0,1,1,1));
		p_renderer->AddLine(glm::vec3(tntl), glm::vec3(tftl), glm::vec4(0,1,0,1));
		p_renderer->AddLine(glm::vec3(tntr), glm::vec3(tftr), glm::vec4(0,1,1,1));
		p_renderer->AddLine(glm::vec3(tnbl), glm::vec3(tfbl), glm::vec4(0,1,0,1));
		p_renderer->AddLine(glm::vec3(tnbr), glm::vec3(tfbr), glm::vec4(0,1,1,1));
		p_renderer->AddLine(glm::vec3(tftl), glm::vec3(tfbl), glm::vec4(0,1,0,1));
		p_renderer->AddLine(glm::vec3(tftr), glm::vec3(tfbr), glm::vec4(0,1,1,1));
	}

	bool Frustum::CheckBoxEx(const AABB& p_box)
	{

		for( unsigned i = 0; i < 6; i++ )
		{
			glm::vec4 plane = glm::vec4(m_planesEx[i].a, m_planesEx[i].b, m_planesEx[i].c, m_planesEx[i].d);

			if(glm::dot(plane, glm::vec4(p_box.m_minX, p_box.m_minY, p_box.m_minZ, 1.0f)) >= 0.0f)
			{
				continue;
			}

			if(glm::dot(plane, glm::vec4(p_box.m_minX, p_box.m_minY, p_box.m_minZ, 1.0f)) >= 0.0f)
			{
				continue;
			}

			if(glm::dot(plane, glm::vec4(p_box.m_maxX, p_box.m_minY, p_box.m_minZ, 1.0f)) >= 0.0f)
			{
				continue;
			}

			if(glm::dot(plane, glm::vec4(p_box.m_minX, p_box.m_minY, p_box.m_minZ, 1.0f)) >= 0.0f)
			{
				continue;
			}

			if(glm::dot(plane, glm::vec4(p_box.m_maxX, p_box.m_minY, p_box.m_maxZ, 1.0f)) >= 0.0f)
			{
				continue;
			}

			if(glm::dot(plane, glm::vec4(p_box.m_minX, p_box.m_maxY, p_box.m_minZ, 1.0f)) >= 0.0f)
			{
				continue;
			}

			if(glm::dot(plane, glm::vec4(p_box.m_maxX, p_box.m_maxY, p_box.m_minZ, 1.0f)) >= 0.0f)
			{
				continue;
			}

			if(glm::dot(plane, glm::vec4(p_box.m_minX, p_box.m_maxY, p_box.m_maxZ, 1.0f)) >= 0.0f)
			{
				continue;
			}

			if(glm::dot(plane, glm::vec4(p_box.m_maxX, p_box.m_maxY, p_box.m_maxZ, 1.0f)) >= 0.0f)
			{
				continue;
			}

      		return false;
		}
		return true;
	}


	bool Frustum::CheckBox(const AABB& p_box)
	{
		return true;
	}

}