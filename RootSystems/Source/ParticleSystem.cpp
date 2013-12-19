#include <RootSystems\Include\ParticleSystem.h>

namespace RootForce
{
	void ParticleSystem::Init()
	{
		m_emitters.Init(m_world->GetEntityManager());
		m_transforms.Init(m_world->GetEntityManager());
	}

	void ParticleSystem::Begin()
	{

	}

	void ParticleSystem::ProcessEntity(ECS::Entity* p_entity)
	{

	}

	void ParticleSystem::End()
	{

	}
}