#ifndef COMPILE_LEVEL_EDITOR
#include "ActionSystem.h"
#include <RootSystems/Include/Script.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootSystems/Include/Network/Messages.h>
#include <RootEngine/Script/Include/RootScript.h>

extern RootEngine::GameSharedContext g_engineContext;
extern RootForce::Network::NetworkEntityMap g_networkEntityMap;

namespace RootSystems
{

	void ActionSystem::Init()
	{
		m_action.Init(m_world->GetEntityManager());
		m_network.Init(m_world->GetEntityManager());
		m_animation.Init(m_world->GetEntityManager());
		m_collision.Init(m_world->GetEntityManager());
		m_transform.Init(m_world->GetEntityManager());
		m_state.Init(m_world->GetEntityManager());
		m_physic.Init(m_world->GetEntityManager());
		m_player.Init(m_world->GetEntityManager());
		m_health.Init(m_world->GetEntityManager());
	}

	void ActionSystem::ProcessEntity( ECS::Entity* p_entity )
	{
		// Get the properties we need.
		float dt = m_world->GetDelta();

		RootForce::Transform* transform = m_transform.Get(p_entity);
		RootForce::Network::NetworkComponent* network = m_network.Get(p_entity);
		RootForce::PlayerPhysics* playphys = m_physic.Get(p_entity);
		RootForce::Collision* collision = m_collision.Get(p_entity);
		RootForce::PlayerComponent* player = m_player.Get(p_entity);
		RootForce::PlayerActionComponent* action = m_action.Get(p_entity);
		RootForce::HealthComponent* health = m_health.Get(p_entity);
		RootForce::StateComponent* state = m_state.Get(p_entity);
		RootForce::Animation* animation = m_animation.Get(p_entity);

		if( health->IsDead )
		{
			if(action->ActivateAbility)
			{
				health->WantsRespawn = true;
				action->ActivateAbility = false;
				action->Jump = false;
				
			}
			animation->m_animClip = RootForce::AnimationClip::RAGDOLL;
			return;
		}

		// Get the facing and calculate the right direction. Facing is assumed to be normalized, and up is assumed to be (0, 1, 0).
		glm::vec3 facing = transform->m_orientation.GetFront();
		glm::vec3 right = transform->m_orientation.GetRight();

		// Calculate movement vector based on input values, the player's speed
		glm::vec3 movement = facing * action->MovePower + right * action->StrafePower;
		if(movement != glm::vec3(0))
		{
			movement = glm::normalize(movement) * playphys->MovementSpeed;
			m_engineContext->m_physics->Move(*(collision->m_handle), movement + transform->m_position);
		}
		
		// Rotate the model and reset the angle
		transform->m_orientation.YawGlobal(action->Angle.x);
		action->Angle.x = 0;

		m_engineContext->m_physics->SetOrientation(*(collision->m_handle), transform->m_orientation.GetQuaternion());
		
		// Activate ability! Pew pew!
		player->SelectedAbility = action->SelectedAbility - 1;
		if(action->ActivateAbility)
		{
			action->ActivateAbility = false;
			
			std::string abilityScript = player->AbilityScripts[player->SelectedAbility];
			if (abilityScript != "")
			{
				m_engineContext->m_script->SetFunction(m_engineContext->m_resourceManager->GetScript(player->AbilityScripts[player->SelectedAbility]), "OnCreate");
				m_engineContext->m_script->AddParameterNumber(network->ID.UserID);
				m_engineContext->m_script->AddParameterNumber(action->ActionID);
				m_engineContext->m_script->ExecuteScript();
			}
		}
		if( animation->m_animClip != RootForce::AnimationClip::RAGDOLL)
		{

		
			if(state->CurrentState == RootForce::EntityState::ASCENDING)
				animation->m_animClip = RootForce::AnimationClip::ASCEND;
			else if(state->CurrentState == RootForce::EntityState::DESCENDING)
				animation->m_animClip = RootForce::AnimationClip::DESCEND;
			else if(state->CurrentState == RootForce::EntityState::LANDING)
			{
				animation->m_animClip = RootForce::AnimationClip::LANDING;
				animation->m_locked = 1;
				state->CurrentState = RootForce::EntityState::GROUNDED;
			}
			else
			{
				//if(action->StrafePower == 0 && action->MovePower == 0)
				animation->m_animClip = RootForce::AnimationClip::IDLE;
			
				if(action->MovePower < 0)
					animation->m_animClip = RootForce::AnimationClip::WALKING;
				else if(action->MovePower > 0)
					animation->m_animClip = RootForce::AnimationClip::WALKING;
				if(action->StrafePower > 0)
					animation->m_animClip = RootForce::AnimationClip::STRAFE_RIGHT;
				else if(action->StrafePower < 0)
					animation->m_animClip = RootForce::AnimationClip::STRAFE_LEFT;
			}
			// Issue a jump if applicable
			if(action->Jump)
			{
				m_engineContext->m_physics->PlayerJump(*(collision->m_handle), playphys->JumpForce);
				if(animation->m_animClip != RootForce::AnimationClip::ASCEND && animation->m_animClip != RootForce::AnimationClip::DESCEND)
				{
					animation->m_animClip = RootForce::AnimationClip::JUMP_START;
					
					animation->m_locked = 1;
				}
				action->Jump = false;
			}

			action->MovePower = 0;
			action->StrafePower = 0;
		}
	}
}

#endif
