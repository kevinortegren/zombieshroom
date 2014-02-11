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

	}

	void SoundSystem::End()
	{

	}

}
