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

		bool isGameOver = false;
		ECS::Entity* matchState = m_world->GetTagManager()->GetEntityByTag("MatchState");
		if(matchState && m_world->GetEntityManager()->GetComponent<RootForce::TDMRuleSet>(matchState)->TimeLeft <= 0)
			isGameOver = true;

		if(!isGameOver)
		{
			if( health->IsDead )
			{
				if(action->WantRespawn)
				{
					health->WantsRespawn = true;
					action->WantRespawn = false;
					action->JumpTime = 0.0f;
					action->AbilityTime = 0.0f;
					player->AbilityState = RootForce::AbilityState::OFF;
				}

				animation->m_animClip = RootForce::AnimationClip::RAGDOLL;
				return;
			}

			// Get the facing and calculate the right direction. Facing is assumed to be normalized, and up is assumed to be (0, 1, 0).
			glm::vec3 facing = transform->m_orientation.GetFront();
			glm::vec3 right = transform->m_orientation.GetRight();
			if(g_engineContext.m_physics->IsOnGround(*collision->m_handle))
			{
				action->JumpDir = facing * action->MovePower + right * action->StrafePower;
			}
			else
			{
				action->JumpDir += facing * action->MovePower + right * action->StrafePower;
			}
				
			if(action->JumpDir != glm::vec3(0))
			{
				if(glm::length2(action->JumpDir) > 1)
					action->JumpDir = glm::normalize(action->JumpDir);
				glm::vec3 movement = action->JumpDir * playphys->MovementSpeed;
				m_engineContext->m_physics->Move(*(collision->m_handle), movement + transform->m_position, dt);
			}

			m_engineContext->m_physics->SetOrientation(*(collision->m_handle), transform->m_orientation.GetQuaternion());



			// Activate ability! Pew pew!
			AbilitySwitch(p_entity);

			// Count down cooldown on all abilities.
			for (unsigned int i = 0; i < PLAYER_NUM_ABILITIES; ++i)
			{
				if (player->AbilityScripts[i].Cooldown > 0)
				{
					player->AbilityScripts[i].Cooldown -= dt;
				}

				if (m_serverPeer != nullptr && player->AbilityScripts[i].OnCooldown && player->AbilityScripts[i].Cooldown <= 0.0f)
				{
					// Cooldown has finished on the server. 
					player->AbilityScripts[i].OnCooldown = false;
					player->AbilityScripts[i].Cooldown = 0.0f;

					//g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::DEBUG_PRINT, "Cooldown on ability %d for player %d is off", i, network->ID.UserID);

					// Send notification about finished cooldown to the client.		
					RootForce::NetworkMessage::AbilityCooldownOff m;
					m.User = network->ID.UserID;
					m.AbilityIndex = i;

					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) ID_TIMESTAMP);
					bs.Write(RakNet::GetTime());
					bs.Write((RakNet::MessageID) RootForce::NetworkMessage::MessageType::AbilityCooldownOff);
					m.Serialize(true, &bs);

					m_serverPeer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
				}
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

			action->JumpTime = 0.0f;
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
		if(!isGameOver)
		{
			if (action->JumpTime > 0.0f)
			{
				if (action->JumpTime >= RootForce::JUMP_TIME_LIMIT)
				{
					action->JumpTime = 0.0f;
				}
				else
				{
					if (g_engineContext.m_physics->IsOnGround(*collision->m_handle))
					{
						// Apply jump force and go into jump animation
						m_engineContext->m_physics->PlayerJump(*(collision->m_handle), playphys->JumpForce);

						if(animation->m_animClip != RootForce::AnimationClip::ASCEND && animation->m_animClip != RootForce::AnimationClip::DESCEND)
						{
							animation->m_animClip = RootForce::AnimationClip::JUMP_START;
							animation->m_locked = 1;
						}
					}
					else
					{
						m_engineContext->m_physics->PlayerJumpBoost(*(collision->m_handle), playphys->JumpBoostForce);
						// TODO: Apply booster jump force
					}
				}
			}
		}

	}

	void ActionSystem::AbilitySwitch(ECS::Entity* p_entity)
	{
		RootForce::PlayerComponent* player = m_player.Get(p_entity);
		RootForce::PlayerActionComponent* action = m_action.Get(p_entity);
		RootForce::Network::NetworkComponent* network = m_network.Get(p_entity);

		player->SelectedAbility = action->SelectedAbility;

		if (action->ActiveAbility != RootForce::ABILITY_INDEX_NONE)
		{
			std::string abilityName = player->AbilityScripts[action->ActiveAbility].Name;
			if (abilityName != "")
			{
				if (player->AbilityScripts[action->ActiveAbility].OnCooldown)
					player->AbilityState = RootForce::AbilityState::OFF;

				float abilityChargeTime = (float) g_engineContext.m_script->GetGlobalNumber("chargeTime", abilityName);
				float abilityChannelingTime = (float) g_engineContext.m_script->GetGlobalNumber("channelingTime", abilityName);
				float abilityCooldownTime = (float) g_engineContext.m_script->GetGlobalNumber("cooldown", abilityName);

		
				switch (player->AbilityState)
				{
				case RootForce::AbilityState::START_CHARGING:
					{
						player->AbilityState = RootForce::AbilityState::CHARGING;

						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::DEBUG_PRINT, "Start charging ability %s (User: %u, Action: %u)", abilityName.c_str(), network->ID.UserID, action->ActionID);
					} break;

				case RootForce::AbilityState::START_CHANNELING:
					{
						player->AbilityState = RootForce::AbilityState::CHANNELING;

						g_engineContext.m_script->SetFunction(m_engineContext->m_resourceManager->GetScript(abilityName), "ChargeDone");
						g_engineContext.m_script->AddParameterNumber(action->AbilityTime);
						g_engineContext.m_script->AddParameterNumber(network->ID.UserID);
						g_engineContext.m_script->AddParameterNumber(action->ActionID);
						g_engineContext.m_script->ExecuteScript();

						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::DEBUG_PRINT, "Start channeling ability %s (User: %u, Action: %u)", abilityName.c_str(), network->ID.UserID, action->ActionID);
					} break;

				case RootForce::AbilityState::STOP_CHANNELING:
					{
						g_engineContext.m_script->SetFunction(m_engineContext->m_resourceManager->GetScript(abilityName), "ChannelingDone");
						g_engineContext.m_script->AddParameterNumber(action->AbilityTime);
						g_engineContext.m_script->AddParameterNumber(network->ID.UserID);
						g_engineContext.m_script->AddParameterNumber(action->ActionID);
						g_engineContext.m_script->ExecuteScript();

						// Put ability on cooldown and decrease charges.
						player->AbilityScripts[action->ActiveAbility].OnCooldown = true;
						player->AbilityScripts[action->ActiveAbility].Cooldown = abilityCooldownTime;

						player->AbilityScripts[action->ActiveAbility].Charges--;
						if(player->AbilityScripts[action->ActiveAbility].Charges == 0)
							player->AbilityScripts[action->ActiveAbility] = RootForce::AbilityInfo();

						// Reset the action component.
						player->AbilityState = RootForce::AbilityState::OFF;
						action->ActiveAbility = RootForce::ABILITY_INDEX_NONE;
						action->AbilityTime = 0.0f;

						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::DEBUG_PRINT, "Stop channeling ability %s (User: %u, Action: %u)", abilityName.c_str(), network->ID.UserID, action->ActionID);
					} break;

				case RootForce::AbilityState::STOP_CHARGING_AND_CHANNELING:
					{
						g_engineContext.m_script->SetFunction(m_engineContext->m_resourceManager->GetScript(abilityName), "ChargeDone");
						g_engineContext.m_script->AddParameterNumber(action->AbilityTime);
						g_engineContext.m_script->AddParameterNumber(network->ID.UserID);
						g_engineContext.m_script->AddParameterNumber(action->ActionID);
						g_engineContext.m_script->ExecuteScript();

						g_engineContext.m_script->SetFunction(m_engineContext->m_resourceManager->GetScript(abilityName), "ChannelingDone");
						g_engineContext.m_script->AddParameterNumber(action->AbilityTime);
						g_engineContext.m_script->AddParameterNumber(network->ID.UserID);
						g_engineContext.m_script->AddParameterNumber(action->ActionID);
						g_engineContext.m_script->ExecuteScript();

						// Put ability on cooldown and decrease charges.
						player->AbilityScripts[action->ActiveAbility].OnCooldown = true;
						player->AbilityScripts[action->ActiveAbility].Cooldown = abilityCooldownTime;

						player->AbilityScripts[action->ActiveAbility].Charges--;
						if(player->AbilityScripts[action->ActiveAbility].Charges == 0)
							player->AbilityScripts[action->ActiveAbility] = RootForce::AbilityInfo();

						// Reset the action component.
						player->AbilityState = RootForce::AbilityState::OFF;
						action->ActiveAbility = RootForce::ABILITY_INDEX_NONE;
						action->AbilityTime = 0.0f;

						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::DEBUG_PRINT, "Stop charging and channeling ability %s (User: %u, Action: %u)", abilityName.c_str(), network->ID.UserID, action->ActionID);
					} break;
				}
			}
		}
	}

	void ActionSystem::UpdateAimingDevice(bool m_inMenu)
	{
		for (RootForce::Network::NetworkEntityMap::iterator it = g_networkEntityMap.begin(); it != g_networkEntityMap.end(); it++)
		{
			if (it->first.ActionID == RootForce::Network::ReservedActionID::CONNECT)
			{
				ECS::Entity* playerEntity = RootForce::Network::FindEntity(g_networkEntityMap, RootForce::Network::NetworkEntityID(it->first.UserID, RootForce::Network::ReservedActionID::CONNECT, RootForce::Network::SEQUENCE_PLAYER_ENTITY));
				if (playerEntity == nullptr)
					continue;

				RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(playerEntity);
				RootForce::PlayerActionComponent* action = m_world->GetEntityManager()->GetComponent<RootForce::PlayerActionComponent>(playerEntity);

				ECS::Entity* aimingDeviceEntity = RootForce::Network::FindEntity(g_networkEntityMap, RootForce::Network::NetworkEntityID(it->first.UserID, RootForce::Network::ReservedActionID::CONNECT, RootForce::Network::SEQUENCE_AIMING_DEVICE_ENTITY));
				if (aimingDeviceEntity == nullptr)
					continue;

				RootForce::Transform* aimingDeviceTransform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(aimingDeviceEntity);

				if(!m_inMenu && action && transform)
				{
					// Rotate the model and reset the angle
					transform->m_orientation.YawGlobal(action->Angle.x);
					action->Angle.x = 0;

					aimingDeviceTransform->m_orientation.SetOrientation(transform->m_orientation.GetQuaternion());
					aimingDeviceTransform->m_orientation.Pitch(action->Angle.y);
				}

				if(transform)
					aimingDeviceTransform->m_position = transform->m_position + transform->m_orientation.GetUp() * 2.0f;
			}
		}
	}

	void ActionSystem::SetServerPeerInterface(RakNet::RakPeerInterface* p_serverPeer)
	{
		m_serverPeer = p_serverPeer;
	}

	void ActionSystem::SetClientPeerInterface(RakNet::RakPeerInterface* p_clientPeer)
	{
		m_clientPeer = p_clientPeer;
	}

}


#endif
