#ifndef COMPILE_LEVEL_EDITOR
#include "StateSystem.h"
#include <Network/NetworkComponents.h>


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
	RootForce::Network::NetworkComponent* network = m_world->GetEntityManager()->GetComponent<RootForce::Network::NetworkComponent>(p_entity);

	
	if(m_engineContext->m_physics->IsOnGround(*(collision->m_handle))
		&& (state->CurrentState == RootForce::EntityState::DESCENDING
			|| state->CurrentState == RootForce::EntityState::ASCENDING)
		&& state->CurrentState != RootForce::EntityState::GROUNDED)
	{
		state->CurrentState = RootForce::EntityState::LANDING;
	}
	else if(!m_engineContext->m_physics->IsOnGround(*(collision->m_handle)))
	{
		float verticalVelocity = g_engineContext.m_physics->GetPlayerVerticalVelocity(*collision->m_handle);

		if (verticalVelocity > 0.0f)
		{
			if (state->CurrentState != RootForce::EntityState::ASCENDING)
			{
				g_engineContext.m_logger->LogText(LogTag::ANIMATION, LogLevel::PINK_PRINT, "(User %u): Switched to ASCENDING (%d) from %d", network->ID.UserID, RootForce::EntityState::ASCENDING, state->CurrentState);
			}

			state->CurrentState = RootForce::EntityState::ASCENDING;
		}
		else if (verticalVelocity < 0.0f)
		{
			if (state->CurrentState != RootForce::EntityState::DESCENDING)
			{
				g_engineContext.m_logger->LogText(LogTag::ANIMATION, LogLevel::PINK_PRINT, "(User %u): Switched to DESCENDING (%d) from %d", network->ID.UserID, RootForce::EntityState::DESCENDING, state->CurrentState);
			}

			state->CurrentState = RootForce::EntityState::DESCENDING;
		}

		/*
		if(state->PrevPosition.y < transform->m_position.y)
		{
			if (state->CurrentState != RootForce::EntityState::ASCENDING)
			{
				g_engineContext.m_logger->LogText(LogTag::ANIMATION, LogLevel::PINK_PRINT, "(User %u): Switched to ASCENDING (%d) from %d", network->ID.UserID, RootForce::EntityState::ASCENDING, state->CurrentState);
			}

			state->CurrentState = RootForce::EntityState::ASCENDING;
		}
		else //if(state->PrevPosition.y > transform->m_position.y)
		{
			if (state->CurrentState != RootForce::EntityState::DESCENDING)
			{
				g_engineContext.m_logger->LogText(LogTag::ANIMATION, LogLevel::PINK_PRINT, "(User %u): Switched to DESCENDING (%d) from %d", network->ID.UserID, RootForce::EntityState::DESCENDING, state->CurrentState);
			}

			state->CurrentState = RootForce::EntityState::DESCENDING;
		}
		*/
	}

	state->PrevPosition = transform->m_position;
}
#endif

