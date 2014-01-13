#include <AnimationSystem.h>

namespace RootForce
{
	void AnimationSystem::Init()
	{
		m_animations.Init(m_world->GetEntityManager());
		m_renderables.Init(m_world->GetEntityManager());
	}

	void AnimationSystem::Begin()
	{
		int a = 0; //??
	}

	void AnimationSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		m_time += m_world->GetDelta();

		if(m_logger)
		{
			Renderable* renderable = m_renderables.Get(p_entity);

			renderable->m_model->m_animations[0]->BoneTransform(m_time);
		}
	}

	void AnimationSystem::End()
	{

	}

	void AnimationSystem::SetLoggingInterface(Logging::LoggingInterface* p_logger)
	{
		m_logger = p_logger;
	}
}