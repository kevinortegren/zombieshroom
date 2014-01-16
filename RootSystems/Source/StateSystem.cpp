#include "StateSystem.h"

void RootSystems::StateSystem::ProcessEntity( ECS::Entity* p_entity )
{
	RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity);
	RootForce::PlayerPhysics* playphys = m_world->GetEntityManager()->GetComponent<RootForce::PlayerPhysics>(p_entity);
	RootForce::Collision* collision = m_world->GetEntityManager()->GetComponent<RootForce::Collision>(p_entity);
	RootForce::StateComponent* state = m_world->GetEntityManager()->GetComponent<RootForce::StateComponent>(p_entity);

	if( !transform || !playphys || !collision || !state )
		return;
	
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

