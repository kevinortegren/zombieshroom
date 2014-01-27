#include "RagdollSystem.h"

namespace RootForce
{
	void RagdollSystem::Init()
	{
		m_animations.Init(m_world->GetEntityManager());
		m_renderables.Init(m_world->GetEntityManager());
	}

	void RagdollSystem::Begin()
	{

	}

	void RagdollSystem::ProcessEntity( ECS::Entity* p_entity )
	{
		Animation* animation = m_animations.Get(p_entity);
		if(animation->m_animClip == AnimationClip::RAGDOLL)
		{
			//Start the ragdoll in Rootphysics;
			//animation->m_bones <- send that to physics and it should do something that should work most likely
		}
		else
		{
			return;
		}
	}

	void RagdollSystem::End()
	{

	}

	

}