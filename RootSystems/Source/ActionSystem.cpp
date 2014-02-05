#ifndef COMPILE_LEVEL_EDITOR
#include "ActionSystem.h"
#include <RootSystems/Include/Script.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootSystems/Include/Network/Messages.h>
#include <RootEngine/Script/Include/RootScript.h>
#include <RootSystems/Include/MatchStateSystem.h>
#include <RakNet/GetTime.h>

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
		
		// Rotate the model and reset the angle
		transform->m_orientation.YawGlobal(action->Angle.x);
		action->Angle.x = 0;

		bool isGameOver = false;
		ECS::Entity* matchState = m_world->GetTagManager()->GetEntityByTag("MatchState");
		if(matchState && m_world->GetEntityManager()->GetComponent<RootForce::TDMRuleSet>(matchState)->TimeLeft <= 0)
			isGameOver = true;

		if(!isGameOver)
		{
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

			m_engineContext->m_physics->SetOrientation(*(collision->m_handle), transform->m_orientation.GetQuaternion());
		
			// Activate ability! Pew pew!
			player->SelectedAbility = action->SelectedAbility - 1;
			if(action->ActivateAbility && player->AbilityScripts[player->SelectedAbility].CooldownOff)
			{
				std::string abilityScript = player->AbilityScripts[player->SelectedAbility].Name;
				if (abilityScript != "")
				{
					m_engineContext->m_script->SetFunction(m_engineContext->m_resourceManager->GetScript(abilityScript), "OnCreate");
					m_engineContext->m_script->AddParameterNumber(network->ID.UserID);
					m_engineContext->m_script->AddParameterNumber(action->ActionID);
					m_engineContext->m_script->ExecuteScript();

					player->AbilityScripts[player->SelectedAbility].CooldownOff = false;

					player->AbilityScripts[player->SelectedAbility].Charges --;
					if(player->AbilityScripts[player->SelectedAbility].Charges == 0)
						player->AbilityScripts[player->SelectedAbility] = RootForce::AbilityInfo();
				}
			}

			// Count down cooldown
			for (unsigned int i = 0; i < PLAYER_NUM_ABILITIES; ++i)
			{
				if (player->AbilityScripts[i].Cooldown > 0)
				{
					player->AbilityScripts[i].Cooldown -= dt;

					if (m_serverPeer != nullptr && player->AbilityScripts[i].Cooldown <= 0.0f)
					{
						// Send notification about finished cooldown to the client.
						RootForce::Network::NetworkComponent* network = m_world->GetEntityManager()->GetComponent<RootForce::Network::NetworkComponent>(p_entity);
						
						RootForce::NetworkMessage::CooldownOff m;
						m.User = network->ID.UserID;
						m.AbilityIndex = i;

						RakNet::BitStream bs;
						bs.Write((RakNet::MessageID) ID_TIMESTAMP);
						bs.Write(RakNet::GetTime());
						bs.Write((RakNet::MessageID) RootForce::NetworkMessage::MessageType::CooldownOff);
						m.Serialize(true, &bs);

						m_serverPeer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, m_serverPeer->GetSystemAddressFromIndex(network->ID.UserID), false);
					}
				}
			}
			
			action->ActivateAbility = false;
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
			
			if(!isGameOver)
			{
				if(action->MovePower < 0)
					animation->m_animClip = RootForce::AnimationClip::WALKING;
				else if(action->MovePower > 0)
					animation->m_animClip = RootForce::AnimationClip::WALKING;
				if(action->StrafePower > 0)
					animation->m_animClip = RootForce::AnimationClip::STRAFE_RIGHT;
				else if(action->StrafePower < 0)
					animation->m_animClip = RootForce::AnimationClip::STRAFE_LEFT;
			}
		}
		// Issue a jump if applicable
		if(!isGameOver && action->Jump)
		{
			m_engineContext->m_physics->PlayerJump(*(collision->m_handle), playphys->JumpForce);
			if(animation->m_animClip != RootForce::AnimationClip::ASCEND && animation->m_animClip != RootForce::AnimationClip::DESCEND)
			{
				animation->m_animClip = RootForce::AnimationClip::JUMP_START;
				animation->m_locked = 1;
			}
			action->Jump = false;
		}

		//action->MovePower = 0;
		//action->StrafePower = 0;
		//UpdateAimingDevice();
	}

	void ActionSystem::UpdateAimingDevice()
	{
		for (RootForce::Network::NetworkEntityMap::iterator it = g_networkEntityMap.begin(); it != g_networkEntityMap.end(); it++)
		{
			if (it->first.ActionID == RootForce::Network::ReservedActionID::CONNECT)
			{
				RootForce::Network::NetworkEntityID id;
				id.UserID = it->first.UserID;
				id.ActionID = RootForce::Network::ReservedActionID::CONNECT;
				id.SequenceID = 0;
				ECS::Entity* playerEntity = g_networkEntityMap[id];
				if (playerEntity == nullptr)
					continue;

				RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(g_networkEntityMap[id]);
				RootForce::PlayerActionComponent* action = m_world->GetEntityManager()->GetComponent<RootForce::PlayerActionComponent>(g_networkEntityMap[id]);

				id.SequenceID = 1;
				ECS::Entity* aimingDeviceEntity = g_networkEntityMap[id];
				if (aimingDeviceEntity == nullptr)
					continue;

				RootForce::Transform* aimingDeviceTransform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(g_networkEntityMap[id]);

				aimingDeviceTransform->m_orientation.SetOrientation(transform->m_orientation.GetQuaternion());
				aimingDeviceTransform->m_orientation.Pitch(action->Angle.y);
				aimingDeviceTransform->m_position = transform->m_position + transform->m_orientation.GetUp() * 2.0f;

			}
		}
	}

	void ActionSystem::SetServerPeerInterface(RakNet::RakPeerInterface* p_serverPeer)
	{
		m_serverPeer = p_serverPeer;
	}

}


#endif
