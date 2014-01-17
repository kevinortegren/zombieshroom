#include "StateSystem.h"


void RootSystems::StateSystem::Init()
{
	m_transform.Init(m_world->GetEntityManager());
	m_state.Init(m_world->GetEntityManager());
	m_physic.Init(m_world->GetEntityManager());
	m_collision.Init(m_world->GetEntityManager());
}


void RootSystems::StateSystem::ProcessEntity( ECS::Entity* p_entity )
{
	RootForce::Transform* transform = m_transform.Get(p_entity);
	RootForce::PlayerPhysics* playphys = m_physic.Get(p_entity);
	RootForce::Collision* collision = m_collision.Get(p_entity);
	RootForce::StateComponent* state = m_state.Get(p_entity);

	
	if(m_engineContext->m_physics->IsOnGround(*(collision->m_handle))
		&& (state->CurrentState == RootForce::EntityState::DESCENDING
			|| state->CurrentState == RootForce::EntityState::ASCENDING)
		&& state->CurrentState != RootForce::EntityState::GROUNDED)
	{
		state->CurrentState = RootForce::EntityState::LANDING;
	}
	else if(!m_engineContext->m_physics->IsOnGround(*(collision->m_handle)))
	{
		if(state->PrevPosition.y < transform->m_position.y)
			state->CurrentState = RootForce::EntityState::ASCENDING;
		else //if(state->PrevPosition.y > transform->m_position.y)
			state->CurrentState = RootForce::EntityState::DESCENDING;
	}

	state->PrevPosition = transform->m_position;
}

