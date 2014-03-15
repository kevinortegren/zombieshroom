#ifndef COMPILE_LEVEL_EDITOR

#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/Network/Messages.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/CameraSystem.h>
#include <RootSystems/Include/PhysicsSystem.h>
#include <RootSystems/Include/Script.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootSystems/Include/CollisionSystem.h>
#include <RootSystems/Include/MatchStateSystem.h>
#include <RootEngine/Script/Include/RootScript.h>
#include <RootSystems/Include/AbilitySpawnSystem.h>
#include <RootSystems/Include/TimerSystem.h>
#include <RootSystems/Include/FollowSystem.h>
#include <RootSystems/Include/HomingSystem.h>
#include <RootSystems/Include/DamageAndKnockback.h>
#include <RootSystems/Include/StatChangeSystem.h>
#include <RootSystems/Include/Components.h>
#include <cstring>

extern RootEngine::GameSharedContext g_engineContext;
extern RootForce::Network::DeletedNetworkEntityList g_networkDeletedList;
extern ECS::World* g_world;

namespace RootForce
{
	namespace NetworkMessage
	{
		void Chat::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, Type);
			p_bs->Serialize(p_writeToBitstream, Sender);
			p_bs->Serialize(p_writeToBitstream, Message);
		}

		void UserConnected::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, User);
			p_bs->Serialize(p_writeToBitstream, IsYou);
			p_bs->Serialize(p_writeToBitstream, Name);
			p_bs->Serialize(p_writeToBitstream, TeamID);
		}

		void UserDisconnected::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, User);
		}

		void UserInformation::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, Name);
		}

		void PlayerCommand::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, User);
			
			p_bs->Serialize(p_writeToBitstream, MovePower);
			p_bs->Serialize(p_writeToBitstream, StrafePower);
			for (int i = 0; i < 2; ++i)
				p_bs->Serialize(p_writeToBitstream, Angle[i]);
			p_bs->Serialize(p_writeToBitstream, JumpTime);
			p_bs->Serialize(p_writeToBitstream, SelectedAbility);

			for (int i = 0; i < 3; ++i)
				p_bs->Serialize(p_writeToBitstream, Position[i]);
			for (int i = 0; i < 4; ++i)
				p_bs->Serialize(p_writeToBitstream, Orientation[i]);
			for (int i = 0; i < 4; ++i)
				p_bs->Serialize(p_writeToBitstream, AimingDeviceOrientation[i]);
		}

		void JumpStart::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, User);
		}

		void JumpStop::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, User);
			p_bs->Serialize(p_writeToBitstream, Time);
		}

		void AbilityEvent::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, User);
			p_bs->Serialize(p_writeToBitstream, Event.ActionID);
			p_bs->Serialize(p_writeToBitstream, Event.Type);
			p_bs->Serialize(p_writeToBitstream, Event.ActiveAbility);
			p_bs->Serialize(p_writeToBitstream, Event.ActiveAbilityScript);
			p_bs->Serialize(p_writeToBitstream, Event.Time);
		}

		void AbilityCooldownOff::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, User);
			p_bs->Serialize(p_writeToBitstream, AbilityIndex);
		}

		void AbilityTryClaim::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, User);
		}

		void AbilityClaimedBy::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, User);
			p_bs->Serialize(p_writeToBitstream, AbilitySpawnPointID);
		}

		void RespawnRequest::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, User);
		}

		void Suicide::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, User);
		}

		void DestroyEntities::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, ID.SynchronizedID);
		}

		void SpawnUser::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, User);
			p_bs->Serialize(p_writeToBitstream, SpawnPointIndex);
		}

		void LoadMap::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, MapName);
		}

		void LoadMapStatus::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, Status);
		}

		void SetMaxPlayers::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, MaxPlayers);
		}

		void SetGameMode::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, GameMode);
		}

		void SetMatchTime::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, Seconds);
		}
		
		void SetKillCount::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, Count);
		}

		void ServerInformation::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, ServerName);
			p_bs->Serialize(p_writeToBitstream, MapName);
			p_bs->Serialize(p_writeToBitstream, AbilityPack);
			p_bs->Serialize(p_writeToBitstream, CurrentPlayers);
			p_bs->Serialize(p_writeToBitstream, MaxPlayers);
			p_bs->Serialize(p_writeToBitstream, PasswordProtected);
			p_bs->Serialize(p_writeToBitstream, IsDedicated);
			p_bs->Serialize(p_writeToBitstream, GameMode);
			p_bs->Serialize(p_writeToBitstream, MatchTimeSeconds);
			p_bs->Serialize(p_writeToBitstream, KillCount);
		}

		void TimeUp::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, ID.SynchronizedID);
		}

		void AbilitySpawn::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, ID);
			p_bs->Serialize(p_writeToBitstream, AbilityName);
		}

		void Death::Serialize( bool p_writeToBitstream, RakNet::BitStream* p_bs )
		{
			p_bs->Serialize(p_writeToBitstream, User);
			p_bs->Serialize(p_writeToBitstream, LastDamageSource);
			p_bs->Serialize(p_writeToBitstream, LastDamageSourceName);
		}

		void PlayerTeamSelect::Serialize( bool p_writeToBitstream, RakNet::BitStream* p_bs )
		{
			p_bs->Serialize(p_writeToBitstream, UserID);
			p_bs->Serialize(p_writeToBitstream, TeamID);
		}

		void PlayerNameChange::Serialize( bool p_writeToBitstream, RakNet::BitStream* p_bs )
		{
			p_bs->Serialize(p_writeToBitstream, Name);
			p_bs->Serialize(p_writeToBitstream, UserID);
		}

		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, Transform* p_c)
		{
			for (int i = 0; i < 3; ++i)
				p_bs->Serialize(p_writeToBitstream, p_c->m_position[i]);
			
			glm::quat q = p_c->m_orientation.GetQuaternion();
			for (int i = 0; i < 4; ++i)
				p_bs->Serialize(p_writeToBitstream, q[i]);
			p_c->m_orientation.SetOrientation(q);

			for (int i = 0; i < 3; ++i)
				p_bs->Serialize(p_writeToBitstream, p_c->m_scale[i]);
		}

		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, HealthComponent* p_c)
		{
			p_bs->Serialize(p_writeToBitstream, p_c->Health);
			p_bs->Serialize(p_writeToBitstream, p_c->LastDamageSourceID);
			p_bs->Serialize(p_writeToBitstream, p_c->IsDead);
			p_bs->Serialize(p_writeToBitstream, p_c->WantsRespawn);
			p_bs->Serialize(p_writeToBitstream, p_c->RespawnDelay);
		}

		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, Physics* p_c)
		{
			p_bs->Serialize(p_writeToBitstream, p_c->m_mass);
			for (int i = 0; i < 3; ++i)
				p_bs->Serialize(p_writeToBitstream, p_c->m_velocity[i]);
		}

		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, Network::NetworkComponent* p_c)
		{
			p_bs->Serialize(p_writeToBitstream, p_c->ID);
		}

		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, LookAtBehavior* p_c)
		{
			p_bs->Serialize(p_writeToBitstream, p_c->m_displacement);

			if (p_writeToBitstream)
			{
				p_bs->Serialize(p_writeToBitstream, RakNet::RakString(p_c->m_targetTag.c_str()) );
			}
			else
			{
				RakNet::RakString s;
				p_bs->Serialize(p_writeToBitstream, s);
				p_c->m_targetTag = std::string(s.C_String());
			}
		}

		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, Script* p_c)
		{
			if (p_writeToBitstream)
			{
				p_bs->Serialize(p_writeToBitstream, RakNet::RakString(p_c->Name.c_str()) );
				
				// Serialize the storage
				unsigned int s;

				s = (unsigned int) p_c->StorageNumber.size();
				p_bs->Serialize(p_writeToBitstream, s);
				for (auto it : p_c->StorageNumber)
				{
					p_bs->Serialize(p_writeToBitstream, RakNet::RakString(it.first.c_str()));
					p_bs->Serialize(p_writeToBitstream, it.second);
				}

				s = (unsigned int) p_c->StorageString.size();
				p_bs->Serialize(p_writeToBitstream, s);
				for (auto it : p_c->StorageString)
				{
					p_bs->Serialize(p_writeToBitstream, RakNet::RakString(it.first.c_str()));
					p_bs->Serialize(p_writeToBitstream, RakNet::RakString(it.second.c_str()));
				}

				s = (unsigned int) p_c->StorageEntity.size();
				p_bs->Serialize(p_writeToBitstream, s);
				for (auto it : p_c->StorageEntity)
				{
					p_bs->Serialize(p_writeToBitstream, RakNet::RakString(it.first.c_str()));
					p_bs->Serialize(p_writeToBitstream, it.second.SynchronizedID);
				}
			}
			else
			{
				RakNet::RakString t;
				p_bs->Serialize(p_writeToBitstream, t);
				p_c->Name = std::string(t.C_String());

				unsigned int s;
				p_bs->Serialize(p_writeToBitstream, s);
				for (unsigned int i = 0; i < s; ++i)
				{
					RakNet::RakString key;
					float value;
					p_bs->Serialize(p_writeToBitstream, key);
					p_bs->Serialize(p_writeToBitstream, value);
					p_c->InsertNumber(key.C_String(), value);
				}

				p_bs->Serialize(p_writeToBitstream, s);
				for (unsigned int i = 0; i < s; ++i)
				{
					RakNet::RakString key;
					RakNet::RakString value;
					p_bs->Serialize(p_writeToBitstream, key);
					p_bs->Serialize(p_writeToBitstream, value);
					p_c->InsertString(key.C_String(), value.C_String());
				}

				p_bs->Serialize(p_writeToBitstream, s);
				for (unsigned int i = 0; i < s; ++i)
				{
					RakNet::RakString key;
					Network::NetworkEntityID value;
					p_bs->Serialize(p_writeToBitstream, key);
					p_bs->Serialize(p_writeToBitstream, value.SynchronizedID);
					p_c->InsertEntity(key.C_String(), value);
				}
			}
		}

		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, PlayerComponent* p_c)
		{
			if (p_writeToBitstream)
			{
				p_bs->Serialize(p_writeToBitstream, RakNet::RakString(p_c->Name.c_str()) );
			}
			else
			{
				RakNet::RakString s;
				p_bs->Serialize(p_writeToBitstream, s);
				p_c->Name = std::string(s.C_String());
			}

			p_bs->Serialize(p_writeToBitstream, p_c->TeamID);

			for (int i = 0; i < PLAYER_NUM_ABILITIES; ++i)
			{
				if (p_writeToBitstream)
				{
					p_bs->Serialize(p_writeToBitstream, RakNet::RakString(p_c->AbilityScripts[i].Name.c_str()) );
					p_bs->Serialize(p_writeToBitstream, p_c->AbilityScripts[i].Cooldown );
					p_bs->Serialize(p_writeToBitstream, p_c->AbilityScripts[i].Charges );
				}
				else
				{
					RakNet::RakString s;
					p_bs->Serialize(p_writeToBitstream, s);
					p_c->AbilityScripts[i].Name = std::string(s.C_String());
					p_bs->Serialize(p_writeToBitstream, p_c->AbilityScripts[i].Cooldown);
					p_bs->Serialize(p_writeToBitstream, p_c->AbilityScripts[i].Charges);
				}
			}
			p_bs->Serialize(p_writeToBitstream, p_c->SelectedAbility);

			p_bs->Serialize(p_writeToBitstream, p_c->Score);
			p_bs->Serialize(p_writeToBitstream, p_c->Deaths);
		}

		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, TDMRuleSet* p_c)
		{
			p_bs->Serialize(p_writeToBitstream, p_c->TimeLeft);
			p_bs->Serialize(p_writeToBitstream, p_c->ScoreLimit);
			for (int i = 0; i < 3; ++i)
				p_bs->Serialize(p_writeToBitstream, p_c->TeamScore[i]);
			p_bs->Serialize(p_writeToBitstream, p_c->MinPlayers);
			p_bs->Serialize(p_writeToBitstream, p_c->CountDown);
			p_bs->Serialize(p_writeToBitstream, p_c->CurrentState);
		}

		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, PlayerPhysics* p_c)
		{
			p_bs->Serialize(p_writeToBitstream, p_c->MovementSpeed);
			p_bs->Serialize(p_writeToBitstream, p_c->JumpForce);
		}

		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, RootForce::AbilitySpawnComponent* p_c)
		{
			p_bs->Serialize(p_writeToBitstream, p_c->Claimed);
			if (p_writeToBitstream)
				{
					p_bs->Serialize(p_writeToBitstream, RakNet::RakString(p_c->CurrentAbility.Name.c_str()) );
					p_bs->Serialize(p_writeToBitstream, p_c->CurrentAbility.Cooldown );
					p_bs->Serialize(p_writeToBitstream, p_c->CurrentAbility.Charges );
				}
				else
				{
					RakNet::RakString s;
					p_bs->Serialize(p_writeToBitstream, s);
					p_c->CurrentAbility.Name = std::string(s.C_String());
					p_bs->Serialize(p_writeToBitstream, p_c->CurrentAbility.Cooldown);
					p_bs->Serialize(p_writeToBitstream, p_c->CurrentAbility.Charges);
				}
			p_bs->Serialize(p_writeToBitstream, p_c->Timer);
		}

		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, TimerComponent* p_c)
		{
			p_bs->Serialize(p_writeToBitstream, p_c->ScriptName);
			p_bs->Serialize(p_writeToBitstream, p_c->FunctionName);
			p_bs->Serialize(p_writeToBitstream, p_c->Target);
			p_bs->Serialize(p_writeToBitstream, p_c->TimeLeft);
			p_bs->Serialize(p_writeToBitstream, p_c->TimeUp);
		}

		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, FollowComponent* p_c)
		{
			p_bs->Serialize(p_writeToBitstream, p_c->Offset);
			p_bs->Serialize(p_writeToBitstream, p_c->TargetID.SynchronizedID);
		}

		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, HomingComponent* p_c)
		{
			p_bs->Serialize(p_writeToBitstream, p_c->Controllability);
			p_bs->Serialize(p_writeToBitstream, p_c->Speed);
			p_bs->Serialize(p_writeToBitstream, p_c->TargetID.SynchronizedID);
			
			for (int i = 0; i < 3; ++i)
				p_bs->Serialize(p_writeToBitstream, p_c->TargetPosition[i]);
		}

		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, DamageAndKnockback* p_c)
		{
			p_bs->Serialize(p_writeToBitstream, p_c->Damage);
			p_bs->Serialize(p_writeToBitstream, p_c->Knockback);
		}

		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, StatChange* p_c)
		{
			p_bs->Serialize(p_writeToBitstream, p_c->DamageResistance);
			p_bs->Serialize(p_writeToBitstream, p_c->JumpHeightChange);
			p_bs->Serialize(p_writeToBitstream, p_c->KnockbackResistance);
			p_bs->Serialize(p_writeToBitstream, p_c->SpeedChange);
		}

		bool CanSerializeComponent(ComponentType::ComponentType p_type)
		{
			switch (p_type)
			{
				case ComponentType::TRANSFORM:
				case ComponentType::HEALTH:
				case ComponentType::PHYSICS:
				case ComponentType::NETWORK:
				case ComponentType::LOOKATBEHAVIOR:
				case ComponentType::SCRIPT:
				case ComponentType::PLAYER:
				case ComponentType::TDMRULES:
				case ComponentType::PLAYERPHYSICS:
				case ComponentType::ABILITYSPAWN:
				case ComponentType::TIMER:
				case ComponentType::FOLLOW:
				case ComponentType::HOMING:
				case ComponentType::DAMAGEANDKNOCKBACK:
				case ComponentType::STATCHANGE:
					return true;
				default:
					return false;
			}
		}

		bool SerializeComponent(RakNet::BitStream* p_bs, ECS::ComponentInterface* p_component, ComponentType::ComponentType p_type)
		{
			if (CanSerializeComponent(p_type))
				p_bs->Serialize(true, p_type);

			switch (p_type)
			{
				case ComponentType::TRANSFORM:
					Serialize(true, p_bs, (RootForce::Transform*) p_component);
				return true;

				case ComponentType::HEALTH:
					Serialize(true, p_bs, (RootForce::HealthComponent*) p_component);
				return true;

				case ComponentType::PHYSICS:
					Serialize(true, p_bs, (RootForce::Physics*) p_component);
				return true;

				case ComponentType::NETWORK:
					Serialize(true, p_bs, (RootForce::Network::NetworkComponent*) p_component);
				return true;

				case ComponentType::LOOKATBEHAVIOR:
					Serialize(true, p_bs, (RootForce::LookAtBehavior*) p_component);
				return true;

				case ComponentType::SCRIPT:
					Serialize(true, p_bs, (RootForce::Script*) p_component);
				return true;

				case ComponentType::PLAYER:
					Serialize(true, p_bs, (RootForce::PlayerComponent*) p_component);
				return true;

				case ComponentType::TDMRULES:
					Serialize(true, p_bs, (RootForce::TDMRuleSet*) p_component);
				return true;

				case ComponentType::PLAYERPHYSICS:
					Serialize(true, p_bs, (RootForce::PlayerPhysics*) p_component);
				return true;

				case ComponentType::ABILITYSPAWN:
					Serialize(true, p_bs, (RootForce::AbilitySpawnComponent*) p_component);
				return true;

				case ComponentType::TIMER:
					Serialize(true, p_bs, (RootForce::TimerComponent*) p_component);
				return true;

				case ComponentType::FOLLOW:
					Serialize(true, p_bs, (RootForce::FollowComponent*) p_component);
				return true;

				case ComponentType::HOMING:
					Serialize(true, p_bs, (RootForce::HomingComponent*) p_component);
				return true;

				case ComponentType::DAMAGEANDKNOCKBACK:
					Serialize(true, p_bs, (RootForce::DamageAndKnockback*) p_component);
				return true;

				case ComponentType::STATCHANGE:
					Serialize(true, p_bs, (RootForce::StatChange*) p_component);
				return true;
			}

			return false;
		}

		template <typename T>
		T* CreateOrGetDeserializedComponent(RakNet::BitStream* p_bs, ECS::Entity* p_entity, ECS::EntityManager* p_entityManager, bool p_discard)
		{
			T* component = nullptr;

			if (p_entity != nullptr)
			{
				component = p_entityManager->GetComponent<T>(p_entity);
				if (component == nullptr)
					component = p_entityManager->CreateComponent<T>(p_entity);

				if (component == nullptr)
					return nullptr;
			
				T c = *component;
				Serialize(false, p_bs, &c);
				if (!p_discard)
				{
					*component = c;
				}
			}
			else
			{
				// If we don't have an entity to deserialize on, just discard the data and move on.
				T c;
				Serialize(false, p_bs, &c);
			}

			return component;
		}

		ECS::ComponentInterface* DeserializeComponent(RakNet::BitStream* p_bs, ECS::Entity* p_entity, ECS::EntityManager* p_entityManager, bool p_isSelf)
		{
			ComponentType::ComponentType type;
			if (!p_bs->Serialize(false, type))
				return nullptr;

			ECS::ComponentInterface* component;
			switch (type)
			{
				case ComponentType::TRANSFORM:
					// Discard if this is our own player entity or aiming device.
					component = CreateOrGetDeserializedComponent<RootForce::Transform>(p_bs, p_entity, p_entityManager, p_isSelf);
				break;

				case ComponentType::HEALTH:
					component = CreateOrGetDeserializedComponent<RootForce::HealthComponent>(p_bs, p_entity, p_entityManager, false);
				break;

				case ComponentType::PHYSICS:
					component = CreateOrGetDeserializedComponent<RootForce::Physics>(p_bs, p_entity, p_entityManager, false);
				break;

				case ComponentType::NETWORK:
					component = CreateOrGetDeserializedComponent<RootForce::Network::NetworkComponent>(p_bs, p_entity, p_entityManager, false);
				break;

				case ComponentType::LOOKATBEHAVIOR:
					component = CreateOrGetDeserializedComponent<RootForce::LookAtBehavior>(p_bs, p_entity, p_entityManager, false);
				break;

				case ComponentType::SCRIPT:
					component = CreateOrGetDeserializedComponent<RootForce::Script>(p_bs, p_entity, p_entityManager, false);
				break;

				case ComponentType::PLAYER:
					component = CreateOrGetDeserializedComponent<RootForce::PlayerComponent>(p_bs, p_entity, p_entityManager, false);
				break;

				case ComponentType::TDMRULES:
					component = CreateOrGetDeserializedComponent<RootForce::TDMRuleSet>(p_bs, p_entity, p_entityManager, false);
				break;

				case ComponentType::PLAYERPHYSICS:
					component = CreateOrGetDeserializedComponent<RootForce::PlayerPhysics>(p_bs, p_entity, p_entityManager, false);
				break;

				case ComponentType::ABILITYSPAWN:
					component = CreateOrGetDeserializedComponent<RootForce::AbilitySpawnComponent>(p_bs, p_entity, p_entityManager, false);
				break;

				case ComponentType::TIMER:
					component = CreateOrGetDeserializedComponent<RootForce::TimerComponent>(p_bs, p_entity, p_entityManager, false);
				break;

				case ComponentType::FOLLOW:
					component = CreateOrGetDeserializedComponent<RootForce::FollowComponent>(p_bs, p_entity, p_entityManager, false);
				break;

				case ComponentType::HOMING:
					component = CreateOrGetDeserializedComponent<RootForce::HomingComponent>(p_bs, p_entity, p_entityManager, false);
				break;

				case ComponentType::DAMAGEANDKNOCKBACK:
					component = CreateOrGetDeserializedComponent<RootForce::DamageAndKnockback>(p_bs, p_entity, p_entityManager, false);
				break;

				case ComponentType::STATCHANGE:
					component = CreateOrGetDeserializedComponent<RootForce::StatChange>(p_bs, p_entity, p_entityManager, false);
				break;
				
				default:
					return nullptr;
			}

			return component;
		}


		bool CanSerializeEntity(ECS::Entity* p_entity, ECS::EntityManager* p_entityManager, const Network::NetworkEntityMap& p_map)
		{
			// Find the entity in the network entity map.
			Network::NetworkEntityMap::const_iterator it;
			for (it = p_map.begin(); it != p_map.end(); it++)
			{
				if (it->second == p_entity)
					break;
			}

			if (it == p_map.end())
				return false;

			// Make sure the entity has a script component.
			Script* script = p_entityManager->GetComponent<Script>(p_entity);
			if (script == nullptr)
			{
				return false;
			}

			return true;
		}

		bool SerializeEntity(RakNet::BitStream* p_bs, ECS::Entity* p_entity, ECS::EntityManager* p_entityManager, const Network::NetworkEntityMap& p_map)
		{
			// Find the entity in the network entity map.
			Network::NetworkEntityMap::const_iterator it;
			for (it = p_map.begin(); it != p_map.end(); it++)
			{
				if (it->second == p_entity)
					break;
			}

			// If it doesn't exist, return false.
			if (it == p_map.end())
			{
				//g_engineContext.m_logger->LogText(LogTag::NETWORK, LogLevel::NON_FATAL_ERROR, "Failed to serialize entity (ID: %d): No associated network entity ID", p_entity->GetId());
				return false;
			}

			// Make sure the entity has a script component.
			Script* script = p_entityManager->GetComponent<Script>(p_entity);
			if (script == nullptr)
			{
				//g_engineContext.m_logger->LogText(LogTag::NETWORK, LogLevel::NON_FATAL_ERROR, "Failed to serialize entity (ID: %d): No script component", p_entity->GetId());
				return false;
			}

#ifdef _DEBUG
			// Make sure the components/flag for the entity is valid.
			if (!AssertEntityValid(p_entityManager, p_entity))
			{
				g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::FATAL_ERROR, "Serialization error. Component/Flag mismatch. Flag: %u. (%u, %u, %u)", p_entity->GetFlag(), it->first.UserID, it->first.ActionID, it->first.SequenceID);
				return false;
			}
#endif

			// Serialize the network entity ID
			p_bs->Serialize(true, it->first);

			// Serialize the script name (that can be used to create this entity if it doesn't exist on the recipient side).
			p_bs->Serialize(true, RakNet::RakString(script->Name.c_str()));

			// Get all components associated with this entity and count the number of components we can serialize.
			std::vector<std::pair<unsigned int, ECS::ComponentInterface*>> components = p_entityManager->GetAllComponents(p_entity);
			
			unsigned int count = 0;
			for (size_t i = 0; i < components.size(); ++i)
			{
				if (CanSerializeComponent((ComponentType::ComponentType)components[i].first))
					++count;
			}

			p_bs->Serialize(true, count);

			// Serialize the components
			for (size_t i = 0; i < components.size(); ++i)
			{
				if (CanSerializeComponent((ComponentType::ComponentType) components[i].first))
					SerializeComponent(p_bs, components[i].second, (ComponentType::ComponentType) components[i].first);
			}

			return true;
		}

		

		ECS::Entity* DeserializeEntity(RakNet::BitStream* p_bs, ECS::EntityManager* p_entityManager, Network::NetworkEntityMap& p_map, Network::UserID_t p_self, std::map<Network::NetworkEntityID, unsigned int>& p_entityOccuranceCount)
		{
			Network::NetworkEntityID id;
			RakNet::RakString scriptName;

			if (!p_bs->Serialize(false, id))
				return nullptr;
			if (!p_bs->Serialize(false, scriptName))
				return nullptr;

			// Increase the occurance counter.
			p_entityOccuranceCount[id]++;

			ECS::Entity* entity = nullptr;
			Network::NetworkEntityMap::const_iterator it = p_map.find(id);
			if (it == p_map.end())
			{
				// See if this entity has been destroyed before, in which case this is an out-dated deserialization message.
				if (std::find(g_networkDeletedList.begin(), g_networkDeletedList.end(), id) == g_networkDeletedList.end())
				{
					// Store the next sequence ID, and set the sequence ID for the next entity to be created to the sent sequence ID.
					Network::UserActionKey_t userActionKey = Network::NetworkComponent::GetUserActionKey(id.UserID, id.ActionID);
					Network::SequenceID_t nextSequenceId = Network::NetworkComponent::s_sequenceIDMap[userActionKey];
					Network::NetworkComponent::s_sequenceIDMap[userActionKey] = id.SequenceID;

					// Entity doesn't exist, use the script to create it.
					g_engineContext.m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Deserializing entity (User: %u, Action: %u, Sequence: %u) with script: \"%s\". Stored next sequence ID: %u.", id.UserID, id.ActionID, id.SequenceID, scriptName.C_String(), nextSequenceId);
					g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->LoadScript(scriptName.C_String()), "OnCreate");
					g_engineContext.m_script->AddParameterNumber(id.UserID);
					g_engineContext.m_script->AddParameterNumber(id.ActionID);
					g_engineContext.m_script->ExecuteScript();

					// Reset the sequence ID map to the highest sequence ID currently used.
					Network::NetworkComponent::s_sequenceIDMap[userActionKey] = std::max(nextSequenceId, Network::NetworkComponent::s_sequenceIDMap[userActionKey]);

					// Get the entity.
					entity = Network::FindEntity(p_map, id);
					assert(entity != nullptr);

					/*
					entity = Network::FindEntity(p_map, id);
					// If entity is not found, assume unsynched sequence ID. Correct the sequence IDs.
					if(entity == nullptr)
					{
						Network::NetworkEntityID tempId = id;
						tempId.SequenceID = Network::NetworkComponent::s_sequenceIDMap[Network::NetworkComponent::GetUserActionKey(id.UserID, id.ActionID)] - 1;
					
						// If received sequence ID is less than that of the local next sequence id, update the local next sequence ID
						if(id.SequenceID > tempId.SequenceID)
							Network::NetworkComponent::s_sequenceIDMap[Network::NetworkComponent::GetUserActionKey(id.UserID, id.ActionID)] = id.SequenceID + 1;

						entity = Network::FindEntity(p_map, tempId);
						assert(entity != nullptr);

						Network::NetworkComponent* netcomp = p_entityManager->GetComponent<Network::NetworkComponent>(entity);
						netcomp->ID = id;

						p_map[id] = entity;
						p_map.erase(p_map.find(tempId));
					}
					*/
				}
				else
				{
					// Skip deserializing components.
					entity = nullptr;
					g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "Got deserialization message for deleted entity, discarding. (User: %u, Action: %u, Sequence: %u)", id.UserID, id.ActionID, id.SequenceID);
				}
			}
			else
			{
				entity = it->second;
				assert(entity != nullptr);
			}

			// Read the number of components
			unsigned int count;
			if (!p_bs->Serialize(false, count))
				return nullptr;

			// Deserialize all components. If the entity is null, just discard all the component data and move on.
			bool isSelf = (id.UserID == p_self) && (id.ActionID == Network::ReservedActionID::CONNECT);
			for (unsigned i = 0; i < count; ++i) 
			{
				if (DeserializeComponent(p_bs, entity, p_entityManager, isSelf) == nullptr)
				{
					// This can happen when an entity is skipped because it has been removed earlier.
					//g_engineContext.m_logger->LogText(LogTag::NETWORK, LogLevel::NON_FATAL_ERROR, "Failed to deserialize component on entity (%d)", entity->GetId());
				}
			}

			#ifdef _DEBUG
			if (entity != nullptr)
			{
				if (!AssertEntityValid(p_entityManager, entity))
				{
					g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::FATAL_ERROR, "Catastrophy: Entity flag component mismatch with flag: %u (%u, %u, %u)", entity->GetFlag(), id.UserID, id.ActionID, id.SequenceID);
				}
			}
			#endif

			return entity;
		}


		bool SortEntitiesToSerialize(ECS::Entity* p_first, ECS::Entity* p_second)
		{
			Network::NetworkComponent* network[2] = { g_world->GetEntityManager()->GetComponent<Network::NetworkComponent>(p_first),
													  g_world->GetEntityManager()->GetComponent<Network::NetworkComponent>(p_second)};

			if (network[0]->ID.UserID < network[1]->ID.UserID)
				return true;
			else if (network[0]->ID.UserID > network[1]->ID.UserID)
				return false;
			else
			{
				if (network[0]->ID.ActionID < network[1]->ID.ActionID)
					return true;
				else if (network[0]->ID.ActionID > network[1]->ID.ActionID)
					return false;
				else
				{
					if (network[0]->ID.SequenceID < network[1]->ID.SequenceID)
						return true;
					else if (network[0]->ID.SequenceID > network[1]->ID.SequenceID)
						return false;
					else
					{
						// No two entities should have the same ID!
						return p_first->GetId() < p_second->GetId();
					}
				}
			}
		}

		void SerializeWorld(RakNet::BitStream* p_bs, ECS::World* p_world, const Network::NetworkEntityMap& p_map)
		{
			// Get all the entities with network components and sort them on UserID, ActionID and SequenceID (in that order).
			std::vector<ECS::Entity*> entities = p_world->GetEntityManager()->GetAllEntities();
			for (size_t i = 0; i < entities.size();)
			{
				Network::NetworkComponent* network = p_world->GetEntityManager()->GetComponent<Network::NetworkComponent>(entities[i]);
				if (network == nullptr)
					entities.erase(entities.begin() + i);
				else
					i++;
			}

			std::sort(entities.begin(), entities.end(), SortEntitiesToSerialize);
			
			// Write the number of serializable entities.
			int count = 0;
			for (size_t i = 0; i < entities.size(); ++i)
			{
				if (CanSerializeEntity(entities[i], p_world->GetEntityManager(), p_map))
					++count;
			}

			p_bs->Serialize(true, count);

			// Serialize all serializable entities.
			for (size_t i = 0; i < entities.size(); ++i)
			{
				SerializeEntity(p_bs, entities[i], p_world->GetEntityManager(), p_map);
			}
		}


		void DeserializeWorld(RakNet::BitStream* p_bs, ECS::World* p_world, Network::NetworkEntityMap& p_map, Network::UserID_t p_self)
		{
			// Read the number of entities
			int count;
			p_bs->Serialize(false, count);

			// Keep track of which entities have been encountered in the serialization message.
			std::map<Network::NetworkEntityID, unsigned int> occuranceCount;

			// Deserialize all entities
			for (int i = 0; i < count; ++i)
			{
				DeserializeEntity(p_bs, p_world->GetEntityManager(), p_map, p_self, occuranceCount);
			}

			// Remove all entities that are in the network entity map but is not in the serialization message.
			for (auto it = p_map.begin(); it != p_map.end();)
			{
				assert(occuranceCount.find(it->first)->second < 2);
				if (occuranceCount.find(it->first)->second == 0)
				{
					// Not encountered in the serialization message. Remove it locally as well.
					g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "Removing local entity (%u, %u, %u) that is non-existant in serialization message.", it->first.UserID, it->first.ActionID, it->first.SequenceID);
					
					p_world->GetEntityManager()->RemoveEntity(it->second);
					g_networkDeletedList.push_back(it->first);
					it = p_map.erase(it);
				}
				else
				{
					it++;
				}
			}
		}
	}
}

#endif
