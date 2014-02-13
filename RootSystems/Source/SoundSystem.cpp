#ifndef COMPILE_LEVEL_EDITOR

#include <RootSystems/Include/SoundSystem.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootEngine/Include/Logging/Logging.h>
extern RootEngine::GameSharedContext g_engineContext;
namespace RootForce
{
	SoundSystem::SoundSystem( ECS::World* p_world, RootEngine::GameSharedContext* p_context )
		: ECS::EntitySystem(p_world), m_context(p_context), m_world(p_world)
	{
		SetUsage<RootForce::Transform>();
		SetUsage<RootForce::SoundComponent>();
	}

	SoundSystem::~SoundSystem()
	{

	}

	void SoundSystem::Init()
	{
		m_transform.Init(m_world->GetEntityManager());
		m_sound.Init(m_world->GetEntityManager());
	}

	void SoundSystem::Begin()
	{
		ECS::Entity* player = m_world->GetTagManager()->GetEntityByTag("Player");
		RootForce::Transform* playerTrans = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(player);
		m_context->m_sound->SetListenerAttributes(playerTrans->m_position, playerTrans->m_orientation.GetUp(), playerTrans->m_orientation.GetFront());
	}

	void SoundSystem::ProcessEntity( ECS::Entity* p_entity )
	{
		RootForce::Transform*		transform = m_transform.Get(p_entity);
		RootForce::SoundComponent*	sound = m_sound.Get(p_entity);

		if (sound->m_soundAudio == nullptr || sound->m_soundChannel == nullptr)
			return;

		if(sound->m_play)
		{
			sound->m_play = false;
			sound->m_soundChannel->PlaySound(sound->m_soundAudio, sound->m_volume);
			sound->m_soundChannel->SetMinMaxDistance(sound->m_minDist, sound->m_maxDist);
		}

		sound->m_soundChannel->SetPosition(transform->m_position);
	}

	void SoundSystem::End()
	{

	}

}
#endif