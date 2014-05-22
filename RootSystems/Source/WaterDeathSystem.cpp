#ifndef COMPILE_LEVEL_EDITOR
#include <RootSystems/Include/WaterDeathSystem.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootSystems/Include/MatchStateSystem.h>
#include <RootSystems/Include/Network/NetworkComponents.h>

extern RootEngine::GameSharedContext g_engineContext;
namespace RootForce
{
	WaterDeathSystem::WaterDeathSystem(ECS::World* p_world) 
		: ECS::EntitySystem(p_world), m_world(p_world)
	{
		SetUsage<RootForce::Transform>();
		SetUsage<RootForce::WaterCollider>();
		SetUsage<RootForce::HealthComponent>();
	}

	WaterDeathSystem::~WaterDeathSystem()
	{
	}

	void WaterDeathSystem::Init()
	{
		m_transform.Init(m_world->GetEntityManager());
		m_waterCollider.Init(m_world->GetEntityManager());
		m_health.Init(m_world->GetEntityManager());
	}

	void WaterDeathSystem::Begin()
	{
	}

	void WaterDeathSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		ECS::Entity* waterEnt = m_world->GetTagManager()->GetEntityByTag("Water");
		if(waterEnt == nullptr)
			return;
		
		RootForce::Transform* waterTrans = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(waterEnt);

		RootForce::Transform*		transform		= m_transform.Get(p_entity);
		RootForce::WaterCollider*	waterCollider	= m_waterCollider.Get(p_entity);
		RootForce::HealthComponent* health			= m_health.Get(p_entity);
		PlayerComponent* playercomp = m_world->GetEntityManager()->GetComponent<RootForce::PlayerComponent>(p_entity);
		Network::NetworkComponent* network = m_world->GetEntityManager()->GetComponent<Network::NetworkComponent>(p_entity);
		
		if(transform->m_position.y < waterTrans->m_position.y - 5.0f && health->Health > 0)
		{
			health->Health = 0;
		}
	}

	void WaterDeathSystem::End()
	{
	}
}
#endif