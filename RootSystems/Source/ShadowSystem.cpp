#include <RootSystems/Include/ShadowSystem.h>

#include <RootEngine/Include/GameSharedContext.h>

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
		sc.m_projectionMatrix = glm::ortho(-200.0f, 200.0f, -200.0f, 200.0f, 0.5f, 400.0f);
		Orientation tOr = transform->m_orientation;
		tOr.Yaw(180.0f);
		glm::mat4 tempWorldMatrix;
		tempWorldMatrix = glm::translate(glm::mat4(1.0f), transform->m_position);
		tempWorldMatrix = glm::rotate(tempWorldMatrix, tOr.GetAngle(), tOr.GetAxis());
		tempWorldMatrix = glm::scale(tempWorldMatrix, transform->m_scale);

		sc.m_viewMatrix = glm::inverse(tempWorldMatrix);

		g_engineContext.m_renderer->AddShadowcaster(sc, m_shadowcasterCount);

		m_shadowcasterCount++;
	}

	void ShadowSystem::End()
	{

	}
}