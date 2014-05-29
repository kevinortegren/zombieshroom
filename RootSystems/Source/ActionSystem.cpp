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
		m_statChange.Init(m_world->GetEntityManager());
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
		RootForce::StatChange* statChange = m_statChange.Get(p_entity);

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
					action->FallTime = 0.0f;
					action->IdleTime = 0.0f;
					action->JumpDir = glm::vec3(0.0f);
					health->GotHit = false;
					animation->UpperBodyAnim.m_locked = false;
					animation->LowerBodyAnim.m_locked = false;


				}

				// Check abilities here as well, to make sure abilities are properly interrupted.
				AbilitySwitch(p_entity);

				animation->UpperBodyAnim.m_animClip = RootForce::AnimationClip::RAGDOLL;
				animation->LowerBodyAnim.m_animClip = RootForce::AnimationClip::RAGDOLL;
				animation->UpperBodyAnim.m_locked = false;
				animation->LowerBodyAnim.m_locked = false;
				//m_engineContext->m_logger->LogText(LogTag::ANIMATION, LogLevel::PINK_PRINT, "RAGDOLL");
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
				glm::vec3 movement = action->JumpDir * playphys->MovementSpeed * statChange->SpeedChange;
				m_engineContext->m_physics->Move(*(collision->m_handle), movement + transform->m_position, dt);
			}

			m_engineContext->m_physics->SetOrientation(*(collision->m_handle), transform->m_orientation.GetQuaternion());

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
							m_engineContext->m_physics->PlayerJump(*(collision->m_handle), playphys->JumpForce * statChange->JumpHeightChange);
							//animation->UpperBodyAnim.SetAnimationClip(RootForce::AnimationClip::JUMP_START, true);
							
							g_engineContext.m_resourceManager->LoadSoundAudio("Movement/JumpingOneshots/jump1-1.wav")->PlayOnce(0.2f, 0x00400011, transform->m_position, 10.0f, 100.0f );
							animation->LowerBodyAnim.SetAnimationClip(RootForce::AnimationClip::JUMP_START, true);
							state->CurrentState = RootForce::EntityState::ASCENDING;
						}
						else
						{
							m_engineContext->m_physics->PlayerJumpBoost(*(collision->m_handle), playphys->JumpBoostForce * statChange->JumpHeightChange * dt);
							// TODO: Apply booster jump force
						}
					}
				}
			}

			if(state->CurrentState == RootForce::EntityState::ASCENDING)
			{
				action->IdleTime = 0.0f;
				action->FallTime = 0.0f;

				if(animation->LowerBodyAnim.m_animClip == RootForce::AnimationClip::IDLE2 || animation->LowerBodyAnim.m_animClip == RootForce::AnimationClip::IDLE3)
				{
					animation->UpperBodyAnim.m_locked = 0;
					animation->LowerBodyAnim.m_locked = 0;
				}

				if(action->StrafePower > 0)
				{
					animation->LowerBodyAnim.SetAnimationClip(RootForce::AnimationClip::AIRSTRAFERIGHT, false);
					animation->UpperBodyAnim.SetAnimationClip(RootForce::AnimationClip::AIRSTRAFERIGHT, false);
				}
				else if(action->StrafePower < 0)
				{
					animation->LowerBodyAnim.SetAnimationClip(RootForce::AnimationClip::AIRSTRAFELEFT, false);
					animation->UpperBodyAnim.SetAnimationClip(RootForce::AnimationClip::AIRSTRAFELEFT, false);
				}
				else
				{
					animation->LowerBodyAnim.SetAnimationClip(RootForce::AnimationClip::ASCEND2, false);
					animation->UpperBodyAnim.SetAnimationClip(RootForce::AnimationClip::ASCEND2, false);
				}
			}
			else if(state->CurrentState == RootForce::EntityState::DESCENDING)
			{
				action->FallTime += dt;
				action->IdleTime = 0.0f;

				if(action->StrafePower > 0)
				{
					animation->LowerBodyAnim.SetAnimationClip(RootForce::AnimationClip::AIRSTRAFERIGHT, false);
					animation->UpperBodyAnim.SetAnimationClip(RootForce::AnimationClip::AIRSTRAFERIGHT, false);
				}
				else if(action->StrafePower < 0)
				{
					animation->LowerBodyAnim.SetAnimationClip(RootForce::AnimationClip::AIRSTRAFELEFT, false);
					animation->UpperBodyAnim.SetAnimationClip(RootForce::AnimationClip::AIRSTRAFELEFT, false);
				}
				else if(action->FallTime < 1)
				{
					animation->UpperBodyAnim.SetAnimationClip(RootForce::AnimationClip::DESCEND3_STRECTH, false);
					animation->LowerBodyAnim.SetAnimationClip(RootForce::AnimationClip::DESCEND3_STRECTH, false);
				}
				else
				{
					animation->UpperBodyAnim.SetAnimationClip(RootForce::AnimationClip::DESCEND, false);
					animation->LowerBodyAnim.SetAnimationClip(RootForce::AnimationClip::DESCEND, false);
				}
			}
			else if(state->CurrentState == RootForce::EntityState::LANDING)
			{
				//animation->LowerBodyAnim.SetAnimationClip(RootForce::AnimationClip::LANDING, true);
				//animation->UpperBodyAnim.SetAnimationClip(RootForce::AnimationClip::JUMP_START, true);

				//std::srand((int)time(NULL));
				//int random = std::rand() % 4;

				//if(random == 0)
					g_engineContext.m_resourceManager->LoadSoundAudio("Movement/JumpingOneshots/jumpland1-1.wav")->PlayOnce(0.2f, 0x00400011, transform->m_position, 10.0f, 100.0f );
				/*else if(random == 1)
					g_engineContext.m_resourceManager->LoadSoundAudio("Movement/JumpingOneshots/jumpland1-2.wav", 0x00400011)->PlayOnce3D(0.2f, transform->m_position, 10.0f, 100.0f );
				else if(random == 2)
					g_engineContext.m_resourceManager->LoadSoundAudio("Movement/JumpingOneshots/jumpland1-3.wav", 0x00400011)->PlayOnce3D(0.2f, transform->m_position, 10.0f, 100.0f );
				else if(random == 3)
					g_engineContext.m_resourceManager->LoadSoundAudio("Movement/JumpingOneshots/jumpland1-4.wav", 0x00400011)->PlayOnce3D(0.2f, transform->m_position, 10.0f, 100.0f );*/

				state->CurrentState = RootForce::EntityState::GROUNDED;
				action->JumpTime = 0.0f;
				action->FallTime = 0.0f;
			}
			else
			{
				if(action->StrafePower == 0 && action->MovePower == 0)
					action->IdleTime += dt;
				else
				{
					action->IdleTime = 0.0f;
				}

				if(action->IdleTime >= 0.2f && animation->LowerBodyAnim.m_animClip != RootForce::AnimationClip::NOCLIP && animation->UpperBodyAnim.m_animClip != RootForce::AnimationClip::NOCLIP)
				{
					animation->UpperBodyAnim.SetAnimationClip(RootForce::AnimationClip::IDLE, false);
					animation->LowerBodyAnim.SetAnimationClip(RootForce::AnimationClip::IDLE, false);
				}else if(animation->LowerBodyAnim.m_animClip == RootForce::AnimationClip::WALKING)
				{
					animation->UpperBodyAnim.SetAnimationClip(RootForce::AnimationClip::IDLE, false);
					animation->LowerBodyAnim.SetAnimationClip(RootForce::AnimationClip::IDLE, false);
				}
				if(action->IdleTime >= 13.0f)
				{
					std::srand((int)time(NULL));
					int random = std::rand() % 2 + 1;

					if(random == 1 && animation->UpperBodyAnim.m_animClip == RootForce::AnimationClip::IDLE && animation->LowerBodyAnim.m_animClip == RootForce::AnimationClip::IDLE &&
									  animation->UpperBodyAnim.m_animClip != RootForce::AnimationClip::IDLE3 && animation->LowerBodyAnim.m_animClip != RootForce::AnimationClip::IDLE3)
					{						
						animation->UpperBodyAnim.SetAnimationClip(RootForce::AnimationClip::IDLE2, true);
						animation->LowerBodyAnim.SetAnimationClip(RootForce::AnimationClip::IDLE2, true);
						action->IdleTime = 0.0f;
					}
					else if(animation->UpperBodyAnim.m_animClip == RootForce::AnimationClip::IDLE && animation->LowerBodyAnim.m_animClip == RootForce::AnimationClip::IDLE &&
							animation->UpperBodyAnim.m_animClip != RootForce::AnimationClip::IDLE2 && animation->LowerBodyAnim.m_animClip != RootForce::AnimationClip::IDLE2)
					{
						animation->UpperBodyAnim.SetAnimationClip(RootForce::AnimationClip::IDLE3, true);
						animation->LowerBodyAnim.SetAnimationClip(RootForce::AnimationClip::IDLE3, true);
						action->IdleTime = 0.0f;
					}						
				}

				if(!isGameOver)
				{
					if(action->MovePower != 0 || action->StrafePower != 0)
					{
						if(animation->LowerBodyAnim.m_animClip == RootForce::AnimationClip::IDLE2 || animation->LowerBodyAnim.m_animClip == RootForce::AnimationClip::IDLE3)
						{
							animation->UpperBodyAnim.m_locked = false;
							animation->LowerBodyAnim.m_locked = false;
							action->IdleTime = 0.0f;
						}
					}

					if(action->MovePower < 0)
					{
						animation->UpperBodyAnim.SetAnimationClip(RootForce::AnimationClip::BACKWARDS, false);
						animation->LowerBodyAnim.SetAnimationClip(RootForce::AnimationClip::BACKWARDS, false);
					}
					else if(action->MovePower > 0)
					{
						animation->UpperBodyAnim.SetAnimationClip(RootForce::AnimationClip::WALKING, false);
						animation->LowerBodyAnim.SetAnimationClip(RootForce::AnimationClip::WALKING, false);
					}
					if(action->StrafePower > 0)
					{
						animation->UpperBodyAnim.SetAnimationClip(RootForce::AnimationClip::STRAFE_RIGHT, false);
						animation->LowerBodyAnim.SetAnimationClip(RootForce::AnimationClip::STRAFE_RIGHT, false);
					}
					else if(action->StrafePower < 0)
					{
						animation->UpperBodyAnim.SetAnimationClip(RootForce::AnimationClip::STRAFE_LEFT, false);
						animation->LowerBodyAnim.SetAnimationClip(RootForce::AnimationClip::STRAFE_LEFT, false);
					}
					if(action->MovePower > 0 && action->StrafePower < 0)
					{
						animation->UpperBodyAnim.SetAnimationClip(RootForce::AnimationClip::LEFTFORWARD, false);
						animation->LowerBodyAnim.SetAnimationClip(RootForce::AnimationClip::LEFTFORWARD, false);
					}
					else if(action->MovePower > 0 && action->StrafePower > 0)
					{
						animation->UpperBodyAnim.SetAnimationClip(RootForce::AnimationClip::RIGHTFORWARD, false);
						animation->LowerBodyAnim.SetAnimationClip(RootForce::AnimationClip::RIGHTFORWARD, false);
					}
					if(action->MovePower < 0 && action->StrafePower < 0)
					{
						animation->UpperBodyAnim.SetAnimationClip(RootForce::AnimationClip::LEFTBACK, false);
						animation->LowerBodyAnim.SetAnimationClip(RootForce::AnimationClip::LEFTBACK, false);
					}
					else if(action->MovePower < 0 && action->StrafePower > 0)
					{
						animation->UpperBodyAnim.SetAnimationClip(RootForce::AnimationClip::RIGHTBACK, false);
						animation->LowerBodyAnim.SetAnimationClip(RootForce::AnimationClip::RIGHTBACK, false);
					}

				}
			}

			//Play hit animation if hit
			if(health->GotHit)
			{
				health->GotHit = false;
				std::srand((int)time(NULL));
				int random = std::rand() % 3 + 1;

				if(random == 1)
					animation->UpperBodyAnim.SetAnimationClip(RootForce::AnimationClip::GOTHIT1, true);
				else if(random == 2)
					animation->UpperBodyAnim.SetAnimationClip(RootForce::AnimationClip::GOTHIT2, true);
				else if(random == 3)
					animation->UpperBodyAnim.SetAnimationClip(RootForce::AnimationClip::GOTHIT3, true);

				action->IdleTime = 0.0f;
			}

			if(!isGameOver)
			{
				if(player->AbilityState == RootForce::AbilityState::CHARGING)
				{
					action->IdleTime = 0.0f;
					if(animation->LowerBodyAnim.m_animClip == RootForce::AnimationClip::IDLE2 || animation->LowerBodyAnim.m_animClip == RootForce::AnimationClip::IDLE3)
					{
						animation->LowerBodyAnim.m_locked = false;
						animation->LowerBodyAnim.SetAnimationClip(RootForce::AnimationClip::IDLE, false);
					}

					if(animation->UpperBodyAnim.m_chargingClip != RootForce::AnimationClip::NOCLIP)
					{
						animation->UpperBodyAnim.m_animClip = animation->UpperBodyAnim.m_chargingClip;
						animation->UpperBodyAnim.m_locked = false;
					}
					if(animation->LowerBodyAnim.m_chargingClip != RootForce::AnimationClip::NOCLIP && animation->LowerBodyAnim.m_animClip == RootForce::AnimationClip::IDLE)
					{
						animation->LowerBodyAnim.m_animClip = animation->LowerBodyAnim.m_chargingClip;
						animation->LowerBodyAnim.m_locked = false;
					}
				}
				else if(player->AbilityState == RootForce::AbilityState::CHANNELING)
				{
					action->IdleTime = 0.0f;
					if(animation->UpperBodyAnim.m_channelingClip != RootForce::AnimationClip::NOCLIP)
					{
						animation->UpperBodyAnim.m_animClip = animation->UpperBodyAnim.m_channelingClip;
						animation->UpperBodyAnim.m_locked = false;
					}
					if(animation->LowerBodyAnim.m_channelingClip != RootForce::AnimationClip::NOCLIP && animation->LowerBodyAnim.m_animClip == RootForce::AnimationClip::IDLE)
					{
						animation->LowerBodyAnim.m_animClip = animation->LowerBodyAnim.m_channelingClip;
						animation->LowerBodyAnim.m_locked = false;
					}
				}
			}

			

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
	}

	void ActionSystem::AbilitySwitch(ECS::Entity* p_entity)
	{
		RootForce::PlayerComponent* player = m_player.Get(p_entity);
		RootForce::PlayerActionComponent* action = m_action.Get(p_entity);
		RootForce::Network::NetworkComponent* network = m_network.Get(p_entity);
		RootForce::Animation* animation = m_animation.Get(p_entity);

		player->SelectedAbility = action->SelectedAbility;

		while (!action->AbilityEvents.empty())
		{
			// Retrieve the first event in the queue.
			const RootForce::AbilityEvent& abilityEvent = action->AbilityEvents.front();

			std::string abilityName = abilityEvent.ActiveAbilityScript.C_String();
			if (abilityName != "")
			{
				g_engineContext.m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Received ability event. Slot %d, Event %d", abilityEvent.ActiveAbility, abilityEvent.Type);

				float abilityCooldownTime = (float) g_engineContext.m_script->GetGlobalNumber("cooldown", abilityName);
				switch (abilityEvent.Type)
				{
					case RootForce::AbilityEventType::CHARGE_START:
					{
						player->AbilityState = RootForce::AbilityState::CHARGING;
						g_engineContext.m_script->SetFunction(m_engineContext->m_resourceManager->GetScript(abilityName), "ChargeStart");
						g_engineContext.m_script->AddParameterNumber((double)network->ID.UserID);
						g_engineContext.m_script->AddParameterNumber((double)abilityEvent.ActionID);
						g_engineContext.m_script->ExecuteScript();

						//g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::PINK_PRINT, "ACTION SYSTEM: Start charging ability %s (User: %u, Action: %u)", abilityName.c_str(), network->ID.UserID, abilityEvent.ActionID);
					} break;

					case RootForce::AbilityEventType::CHANNELING_START:
					{
						player->AbilityState = RootForce::AbilityState::CHANNELING;
						
						g_engineContext.m_script->SetFunction(m_engineContext->m_resourceManager->GetScript(abilityName), "ChargeDone");
						g_engineContext.m_script->AddParameterNumber((double)abilityEvent.Time);
						g_engineContext.m_script->AddParameterNumber((double)network->ID.UserID);
						g_engineContext.m_script->AddParameterNumber((double)abilityEvent.ActionID);
						g_engineContext.m_script->ExecuteScript();

						// DEBUG
						// Get the new entity and print its flag
						//ECS::Entity* entity = RootForce::Network::FindEntity(g_networkEntityMap, RootForce::Network::NetworkEntityID(network->ID.UserID, abilityEvent.ActionID, 0));
						//g_engineContext.m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "%s (User: %u, Action: %u) created with flag: %llx", abilityName.c_str(), network->ID.UserID, abilityEvent.ActionID, entity->GetFlag());

						// /DEBUG

						//g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::PINK_PRINT, "ACTION SYSTEM: Start channeling ability %s (User: %u, Action: %u)", abilityName.c_str(), network->ID.UserID, abilityEvent.ActionID);
					} break;

					case RootForce::AbilityEventType::CHANNELING_DONE:
					{
						player->AbilityState = RootForce::AbilityState::OFF;

						// Put ability on cooldown and decrease charges.
						player->AbilityScripts[abilityEvent.ActiveAbility].OnCooldown = true;
						player->AbilityScripts[abilityEvent.ActiveAbility].Cooldown = abilityCooldownTime;

						player->AbilityScripts[abilityEvent.ActiveAbility].Charges--;
						
						g_engineContext.m_script->SetFunction(m_engineContext->m_resourceManager->GetScript(abilityName), "ChannelingDone");
						g_engineContext.m_script->AddParameterNumber(abilityEvent.Time);
						g_engineContext.m_script->AddParameterNumber(network->ID.UserID);
						g_engineContext.m_script->AddParameterNumber(abilityEvent.ActionID);
						g_engineContext.m_script->ExecuteScript();

						if(player->AbilityScripts[abilityEvent.ActiveAbility].Charges == 0)
							player->AbilityScripts[abilityEvent.ActiveAbility] = RootForce::AbilityInfo();

						animation->UpperBodyAnim.m_channelingClip = RootForce::AnimationClip::NOCLIP;
						animation->LowerBodyAnim.m_channelingClip = RootForce::AnimationClip::NOCLIP;
						animation->UpperBodyAnim.m_chargingClip = RootForce::AnimationClip::NOCLIP;
						animation->LowerBodyAnim.m_chargingClip = RootForce::AnimationClip::NOCLIP;
						//g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::PINK_PRINT, "ACTION SYSTEM: Stop channeling ability %s (User: %u, Action: %u)", abilityName.c_str(), network->ID.UserID, abilityEvent.ActionID);
					} break;

					case RootForce::AbilityEventType::INTERRUPTED:
					{
						player->AbilityState = RootForce::AbilityState::OFF;

						g_engineContext.m_script->SetFunction(m_engineContext->m_resourceManager->GetScript(abilityName), "Interrupted");
						g_engineContext.m_script->AddParameterNumber(abilityEvent.Time);
						g_engineContext.m_script->AddParameterNumber(network->ID.UserID);
						g_engineContext.m_script->AddParameterNumber(abilityEvent.ActionID);
						g_engineContext.m_script->ExecuteScript();

						// Put ability on cooldown and decrease charges.
						player->AbilityScripts[abilityEvent.ActiveAbility].OnCooldown = true;
						player->AbilityScripts[abilityEvent.ActiveAbility].Cooldown = abilityCooldownTime;

						player->AbilityScripts[abilityEvent.ActiveAbility].Charges--;
						if(player->AbilityScripts[abilityEvent.ActiveAbility].Charges == 0)
							player->AbilityScripts[abilityEvent.ActiveAbility] = RootForce::AbilityInfo();

						animation->UpperBodyAnim.m_channelingClip = RootForce::AnimationClip::NOCLIP;
						animation->LowerBodyAnim.m_channelingClip = RootForce::AnimationClip::NOCLIP;
						animation->UpperBodyAnim.m_chargingClip = RootForce::AnimationClip::NOCLIP;
						animation->LowerBodyAnim.m_chargingClip = RootForce::AnimationClip::NOCLIP;
						//g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::PINK_PRINT, "ACTION SYSTEM: Interrupted ability %s (User: %u, Action: %u)", abilityName.c_str(), network->ID.UserID, abilityEvent.ActionID);
					} break;
				}
			}
			else
			{
				g_engineContext.m_logger->LogText(LogTag::GAME, LogLevel::WARNING, "Received ability event for empty slot. Slot %d, Event: %d", abilityEvent.ActiveAbility, abilityEvent.Type);
			}

			// Pop the event.
			action->AbilityEvents.pop();
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
				RootForce::HealthComponent*	health	= m_world->GetEntityManager()->GetComponent<RootForce::HealthComponent>(playerEntity);
				ECS::Entity* aimingDeviceEntity = RootForce::Network::FindEntity(g_networkEntityMap, RootForce::Network::NetworkEntityID(it->first.UserID, RootForce::Network::ReservedActionID::CONNECT, RootForce::Network::SEQUENCE_AIMING_DEVICE_ENTITY));
				if (aimingDeviceEntity == nullptr)
					continue;

				RootForce::Transform* aimingDeviceTransform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(aimingDeviceEntity);

				if(!m_inMenu && action && transform)//assuming there is a health component
				{
					if(health)
					{
						if(health->Health > 0)
						{
							// Rotate the model and reset the angle
							transform->m_orientation.YawGlobal(action->Angle.x);
							action->Angle.x = 0;

							aimingDeviceTransform->m_orientation.SetOrientation(transform->m_orientation.GetQuaternion());
							aimingDeviceTransform->m_orientation.Pitch(action->Angle.y);
						}
						else //Dead
						{
							aimingDeviceTransform->m_orientation.YawGlobal(action->Angle.x);
							action->Angle.x = 0;
							aimingDeviceTransform->m_orientation.Pitch(action->Angle.y);
							action->Angle.y = 0;
						}
					}
				}

				if(transform)
				{
					if(health)
					{
						if(health->Health > 0)
							aimingDeviceTransform->m_position = transform->m_position + transform->m_orientation.GetUp() * 2.0f;
						else //Dead
							aimingDeviceTransform->m_position = transform->m_position;
					}
					else
					{//no health component
						aimingDeviceTransform->m_position = transform->m_position + transform->m_orientation.GetUp() * 2.0f;
					}
				}
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
