#include "ActionSystem.h"
#include <RootSystems/Include/Script.h>
#include <RootSystems/Include/Network/NetworkEntityMap.h>

extern RootEngine::GameSharedContext g_engineContext;

namespace RootSystems
{

	void ActionSystem::Init()
	{
		m_action.Init(m_world->GetEntityManager());
		m_animation.Init(m_world->GetEntityManager());
		m_collision.Init(m_world->GetEntityManager());
		m_transform.Init(m_world->GetEntityManager());
		m_state.Init(m_world->GetEntityManager());
		m_physic.Init(m_world->GetEntityManager());
		m_ability.Init(m_world->GetEntityManager());
		m_health.Init(m_world->GetEntityManager());
	}

	void ActionSystem::ProcessEntity( ECS::Entity* p_entity )
	{
		// Get the properties we need.
		float dt = m_world->GetDelta();

		RootForce::Transform* transform = m_transform.Get(p_entity);
		RootForce::PlayerPhysics* playphys = m_physic.Get(p_entity);
		RootForce::Collision* collision = m_collision.Get(p_entity);
		RootForce::UserAbility* ability = m_ability.Get(p_entity);
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
		ability->SelectedAbility = ability->Abilities[action->SelectedAbility-1];
		if(action->ActivateAbility)
		{
			action->ActivateAbility = false;
			switch(ability->SelectedAbility)
			{
			case RootForce::Ability::ABILITY_TEST:
				{
					/*
					ECS::Entity* entity = m_world->GetEntityManager()->CreateEntity();
					RootForce::Script* script = m_world->GetEntityManager()->CreateComponent<RootForce::Script>(entity);
					script->m_name = m_engineContext->m_resourceManager->GetScript("AbilityTest");
					script->m_actions.push_back(RootForce::Action(RootForce::ActionType::ACTION_CREATE));
					*/

					m_engineContext->m_script->SetFunction(m_engineContext->m_resourceManager->GetScript("AbilityTest"), "OnCreate");
					m_engineContext->m_script->ExecuteScript();
				}
				break;
			default:
				break;
			}
		}

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
			if(action->MovePower == -1)
				animation->m_animClip = RootForce::AnimationClip::WALKING;
			else if(action->MovePower == 1)
				animation->m_animClip = RootForce::AnimationClip::WALKING;
			if(action->StrafePower == 1)
				animation->m_animClip = RootForce::AnimationClip::STRAFE_RIGHT;
			else if(action->StrafePower == -1)
				animation->m_animClip = RootForce::AnimationClip::STRAFE_LEFT;
		}
		// Issue a jump if applicable
		if(action->Jump)
		{
			m_engineContext->m_physics->PlayerJump(*(collision->m_handle), playphys->JumpForce);
			animation->m_animClip = RootForce::AnimationClip::JUMP_START;
			animation->m_locked = 1;
			action->Jump = false;
		}

	}

}