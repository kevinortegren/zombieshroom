#include <RootForce\Include\LightSystem.h>

namespace RootForce
{	
	void DirectionalLightSystem::Init()
	{
		m_dlights.Init(m_world->GetEntityManager());
		m_transforms.Init(m_world->GetEntityManager());
	}

	void DirectionalLightSystem::Begin()
	{

	}

	void DirectionalLightSystem::ProcessEntity(ECS::Entity* p_entity, float dt)
	{

	}

	void DirectionalLightSystem::End()
	{
		
	}

	void PointLightSystem::Init()
	{
		m_plights.Init(m_world->GetEntityManager());
		m_transforms.Init(m_world->GetEntityManager());
	}	

	void PointLightSystem::Begin()
	{
		lightCount = 0;
	}

	void PointLightSystem::ProcessEntity(ECS::Entity* p_entity, float dt)
	{
		Transform* transform = m_transforms.Get(p_entity);
		PointLight* pointLight = m_plights.Get(p_entity);

		Render::PointLight pl;
		pl.m_position = transform->m_position;
		pl.m_color = pointLight->m_color;
		pl.m_attenuation = pointLight->m_attenuation;
		pl.m_range = pointLight->m_range;

		m_renderer->AddPointLight(pl, lightCount);

		lightCount++;
	}

	void PointLightSystem::End()
	{
		lightCount = 0;
	}
}