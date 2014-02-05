#ifndef COMPILE_LEVEL_EDITOR
#include <RootSystems/Include/WaterCollisionSystem.h>
#include <RootEngine/Include/Logging/Logging.h>

namespace RootSystems
{
	WaterCollsionSystem::WaterCollsionSystem( ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext, RootForce::WaterSystem* p_waterSystem) : 
		ECS::EntitySystem(p_world), m_engineContext(p_engineContext), m_waterSystem(p_waterSystem)
	{
		SetUsage<RootForce::Transform>();
		SetUsage<RootForce::WaterCollider>();
	}

	void WaterCollsionSystem::Init()
	{
		m_transform.Init(m_world->GetEntityManager());
		m_waterCollider.Init(m_world->GetEntityManager());
	}

	void WaterCollsionSystem::ProcessEntity( ECS::Entity* p_entity )
	{
		RootForce::Transform*		transform = m_transform.Get(p_entity);
		RootForce::WaterCollider*	waterCollider = m_waterCollider.Get(p_entity);

		float waterHeight = m_waterSystem->GetWaterHeight();

		//If over water, set edge time to 0 and return
		if(transform->m_position.y > waterHeight + waterCollider->m_radius)
		{
			waterCollider->m_waterState = RootForce::WaterState::WaterState::OVER_WATER;
			waterCollider->m_edgeWaterTime = 0.0f;
			return;
		} //if under water, set edge time to 0 and return
		else if(transform->m_position.y < waterHeight - waterCollider->m_radius)
		{
			waterCollider->m_waterState = RootForce::WaterState::WaterState::UNDER_WATER;
			waterCollider->m_edgeWaterTime = 0.0f;
			return;
		}
		else //if by the edge of the water, start disturbing at given interval
			waterCollider->m_waterState = RootForce::WaterState::WaterState::EDGE_WATER;

		if(waterCollider->m_edgeWaterTime <= 0.0f && glm::distance(glm::vec2(waterCollider->m_prevPos.x, waterCollider->m_prevPos.z) , glm::vec2(transform->m_position.x, transform->m_position.z)) > 20.0f )
		{	//Disturb
			m_waterSystem->Disturb(transform->m_position.x, transform->m_position.z, waterCollider->m_disturbPower);
			waterCollider->m_prevPos = transform->m_position;
			waterCollider->m_edgeWaterTime = waterCollider->m_disturbInterval;
		}
		//Decrease edge time
		waterCollider->m_edgeWaterTime -= m_world->GetDelta();
	}
}

#endif

