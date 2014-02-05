#include <LightSystem.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootSystems/Include/CameraSystem.h>

namespace RootForce
{	
	void PointLightSystem::Init()
	{
		m_plights.Init(m_world->GetEntityManager());
		m_transforms.Init(m_world->GetEntityManager());
	}	

	void PointLightSystem::Begin()
	{
		m_lightCount = 0;
		
		// Fetch camera frustum.
		auto entity = m_world->GetTagManager()->GetEntityByTag("Camera");
		m_frustum = &m_world->GetEntityManager()->GetComponent<RootForce::Camera>(entity)->m_frustum;
	}

	void PointLightSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		Transform* transform = m_transforms.Get(p_entity);
		PointLight* pointLight = m_plights.Get(p_entity);

		if(m_frustum->CheckSphere(transform->m_position, pointLight->m_range))
		{
			Render::PointLight pl;
			pl.m_position = transform->m_position;
			pl.m_color = pointLight->m_color;
			pl.m_attenuation = pointLight->m_attenuation;
			pl.m_range = pointLight->m_range;

			m_context->m_renderer->AddPointLight(pl, m_lightCount);

			m_lightCount++;
		}

		
	}

	void PointLightSystem::End() 
	{
		std::cout << m_lightCount << std::endl;
	}

	void DirectionalLightSystem::Init()
	{
		m_dlights.Init(m_world->GetEntityManager());
		m_transforms.Init(m_world->GetEntityManager());
	}

	void DirectionalLightSystem::Begin() 
	{ 
		m_lightCount = 0; 
	}

	void DirectionalLightSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		Transform* transform = m_transforms.Get(p_entity);
		DirectionalLight* directionalLigt = m_dlights.Get(p_entity);

		Render::DirectionalLight dl;
		dl.m_color = directionalLigt->m_color;
		dl.m_direction = transform->m_orientation.GetFront();

		m_context->m_renderer->AddDirectionalLight(dl, m_lightCount);

		m_lightCount++;
	}

	void DirectionalLightSystem::End() {}
}