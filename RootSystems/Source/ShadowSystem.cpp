#include <RootSystems/Include/ShadowSystem.h>

namespace RootForce
{
	void ShadowSystem::Init()
	{
		m_shadowcasters.Init(m_world->GetEntityManager());
		m_transforms.Init(m_world->GetEntityManager());
	}

	void ShadowSystem::Begin()
	{

	}

	void ShadowSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		Transform* transform = m_transforms.Get(p_entity);
		Shadowcaster* shadowcaster = m_shadowcasters.Get(p_entity);

		Render::Shadowcaster sc;
		//sc.m_projectionMatrix = glm::perspectiveFov(shadowcaster->m_frustum.)

		//g_engineContext.m_renderer->AddShadowcaster(sc, m_shadowcasterCount);

		//m_shadowcasterCount++;
	}

	void ShadowSystem::End()
	{

	}
}