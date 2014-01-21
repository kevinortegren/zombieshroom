#ifndef COMPILE_LEVEL_EDITOR

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
#include <cstring>

extern RootEngine::GameSharedContext g_engineContext;

namespace RootForce
{
	namespace NetworkMessage
	{
		GameStateDelta::SerializableComponent::SerializableComponent()
			: Data(nullptr) {}

		GameStateDelta::SerializableComponent::~SerializableComponent()
		{
			delete [] Data;
		}


		void GameStateDelta::SerializableComponent::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, Type);
			p_bs->Serialize(p_writeToBitstream, DataSize);

			if (!p_writeToBitstream)
				Data = new char[DataSize];
			p_bs->Serialize(p_writeToBitstream, Data, DataSize);
		}


		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, Transform* p_c)
		{
			for (int i = 0; i < 3; ++i)
				p_bs->Serialize(p_writeToBitstream, p_c->m_position[i]);
			for (int i = 0; i < 4; ++i)
				p_bs->Serialize(p_writeToBitstream, p_c->m_orientation.GetQuaternion()[i]);
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
			}
			else
			{
				RakNet::RakString s;
				p_bs->Serialize(p_writeToBitstream, s);
				p_c->Name = std::string(s.C_String());
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
					p_bs->Serialize(p_writeToBitstream, RakNet::RakString(p_c->AbilityScripts[i].c_str()) );
				}
				else
				{
					RakNet::RakString s;
					p_bs->Serialize(p_writeToBitstream, s);
					p_c->AbilityScripts[i] = std::string(s.C_String());
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
		}

		void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs, PlayerPhysics* p_c)
		{
			p_bs->Serialize(p_writeToBitstream, p_c->MovementSpeed);
			p_bs->Serialize(p_writeToBitstream, p_c->JumpForce);
		}
		
		

		bool GameStateDelta::SerializableComponent::SerializeComponent(ECS::ComponentInterface* p_component, ComponentType::ComponentType p_type)
		{
			Type = p_type;
			
			bool success = false;
			RakNet::BitStream bs;
			switch (p_type)
			{
				case ComponentType::TRANSFORM:
				{
					Transform* c = (Transform*) p_component;
					
					RootForce::NetworkMessage::Serialize(true, &bs, c);
					success = true;
				} break;

				case ComponentType::HEALTH:
				{
					HealthComponent* c = (HealthComponent*) p_component;

					RootForce::NetworkMessage::Serialize(true, &bs, c);
					success = true;
				} break;

				case ComponentType::PHYSICS:
				{
					Physics* c = (Physics*) p_component;

					RootForce::NetworkMessage::Serialize(true, &bs, c);
					success = true;
				} break;

				case ComponentType::NETWORK:
				{
					Network::NetworkComponent* c = (Network::NetworkComponent*) p_component;

					RootForce::NetworkMessage::Serialize(true, &bs, c);
					success = true;
				} break;

				case ComponentType::LOOKATBEHAVIOR:
				{
					LookAtBehavior* c = (LookAtBehavior*) p_component;

					RootForce::NetworkMessage::Serialize(true, &bs, c);
					success = true;
				} break;

				case ComponentType::SCRIPT:
				{
					Script* c = (Script*) p_component;
					
					RootForce::NetworkMessage::Serialize(true, &bs, c);
					success = true;
				} break;

				case ComponentType::PLAYER:
				{
					PlayerComponent* c = (PlayerComponent*) p_component;

					RootForce::NetworkMessage::Serialize(true, &bs, c);
					success = true;
				} break;

				case ComponentType::TDMRULES:
				{
					TDMRuleSet* c = (TDMRuleSet*) p_component;

					RootForce::NetworkMessage::Serialize(true, &bs, c);
					success = true;
				} break;

				case ComponentType::PLAYERPHYSICS:
				{
					PlayerPhysics* c = (PlayerPhysics*) p_component;

					RootForce::NetworkMessage::Serialize(true, &bs, c);
					success = true;
				} break;
			}

			DataSize = bs.GetNumberOfBytesUsed();
			RakNet::BitSize_t bitsize = bs.CopyData((unsigned char**)&Data);

			assert( (int)std::ceil(bitsize / 8.0f) == DataSize);

			return success;
		}

		template <typename T>
		T* CreateOrGetAndDeserializeComponent(ECS::Entity* p_entity, ECS::EntityManager* p_entityManager)
		{
			T* c = p_entityManager->GetComponent<T>(p_entity);
			if (c == nullptr)
				p_entityManager->CreateComponent<T>(p_entity);

			RootForce::NetworkMessage::Serialize(false, &bs, c);
		}

		ECS::ComponentInterface* GameStateDelta::SerializableComponent::DeserializeComponent(ECS::Entity* p_entity, ECS::EntityManager* p_entityManager)
		{
			RakNet::BitStream bs((unsigned char*)Data, DataSize, false);

			switch (Type)
			{
				case ComponentType::TRANSFORM:
				{
					CreateOrGetAndDeserializeComponent<Transform>(p_entity, p_entityManager);
				} break;

				case ComponentType::HEALTH:
				{
					CreateOrGetAndDeserializeComponent<HealthComponent>(p_entity, p_entityManager);
				} break;

				case ComponentType::PHYSICS:
				{
					CreateOrGetAndDeserializeComponent<Physics>(p_entity, p_entityManager);
				} break;

				case ComponentType::NETWORK:
				{
					CreateOrGetAndDeserializeComponent<Network::NetworkComponent>(p_entity, p_entityManager);
				} break;

				case ComponentType::LOOKATBEHAVIOR:
				{
					CreateOrGetAndDeserializeComponent<LookAtBehavior>(p_entity, p_entityManager);
				} break;

				case ComponentType::SCRIPT:
				{
					CreateOrGetAndDeserializeComponent<Script>(p_entity, p_entityManager);
				} break;

				case ComponentType::PLAYER:
				{
					CreateOrGetAndDeserializeComponent<PlayerComponent>(p_entity, p_entityManager);
				} break;

				case ComponentType::TDMRULES:
				{
					CreateOrGetAndDeserializeComponent<TDMRuleSet>(p_entity, p_entityManager);
				} break;

				case ComponentType::PLAYERPHYSICS:
				{
					CreateOrGetAndDeserializeComponent<PlayerPhysics>(p_entity, p_entityManager);
				} break;
			}
		}


		void GameStateDelta::SerializableEntity::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			p_bs->Serialize(p_writeToBitstream, ID);
			p_bs->Serialize(p_writeToBitstream, ScriptName);
			
			int size = Components.size();
			p_bs->Serialize(p_writeToBitstream, size);

			if (!p_writeToBitstream)
				Components.resize(size);

			for (int i = 0; i < Components.size(); ++i)
			{
				Components[i].Serialize(p_writeToBitstream, p_bs);
			}
		}

		bool GameStateDelta::SerializableEntity::SerializeEntity(ECS::Entity* p_entity, ECS::World* p_world, const Network::NetworkEntityMap& p_map)
		{
			Network::NetworkEntityMap::const_iterator it = std::find(p_map.begin(), p_map.end(), p_entity);
			if (it == p_map.end())
			{
				g_engineContext.m_logger->LogText(LogTag::NETWORK, LogLevel::NON_FATAL_ERROR, "Failed to serialize entity (ID: %d): No associated network entity ID", p_entity->GetId());
				return false;
			}

			Script* script = p_world->GetEntityManager()->GetComponent<Script>(p_entity);
			if (script == nullptr)
			{
				g_engineContext.m_logger->LogText(LogTag::NETWORK, LogLevel::NON_FATAL_ERROR, "Failed to serialize entity (ID: %d): No script component", p_entity->GetId());
				return false;
			}

			ID = it->first;
			ScriptName = RakNet::RakString(script->Name.c_str());

			std::vector<std::pair<unsigned int, ECS::ComponentInterface*>> components = p_world->GetEntityManager()->GetAllComponents(p_entity);
			for (size_t i = 0; i < components.size(); ++i)
			{
				SerializableComponent c;
				if (c.SerializeComponent(components[i].second, (ComponentType::ComponentType)components[i].first))
				{
					Components.push_back(c);
				}
			}

			return true;
		}

		ECS::Entity* GameStateDelta::SerializableEntity::DeserializeEntity(ECS::World* p_world, Network::NetworkEntityMap& p_map)
		{
			ECS::Entity* entity;
			Network::NetworkEntityMap::const_iterator it = p_map.find(ID);
			
			if (it == p_map.end())
			{
				// Entity doesn't exist, so create it.
				entity = p_world->GetEntityManager()->CreateEntity();

				g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->GetScript(ScriptName.C_String()), "OnCreate");
				g_engineContext.m_script->AddParameterUserData(entity, sizeof(ECS::Entity*), "Entity");
				g_engineContext.m_script->AddParameterNumber(ID.UserID);
				g_engineContext.m_script->AddParameterNumber(ID.ActionID);
				g_engineContext.m_script->ExecuteScript();

				g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->GetScript(ScriptName.C_String()), "AddClientComponents");
				g_engineContext.m_script->AddParameterUserData(entity, sizeof(ECS::Entity*), "Entity");
				g_engineContext.m_script->ExecuteScript();
			}
			else
			{
				entity = it->second;
			}

			// Deserialize components
			for (size_t i = 0; i < Components.size(); ++i)
			{

			}

			return entity;
		}

		void GameStateDelta::Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs)
		{
			int size = Entities.size();
			p_bs->Serialize(p_writeToBitstream, size);

			if (!p_writeToBitstream)
				Entities.resize(size);

			for (int i = 0; i < Entities.size(); ++i)
			{
				Entities[i].Serialize(p_writeToBitstream, p_bs);
			}
		}


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
			p_bs->Serialize(p_writeToBitstream, Action.MovePower);
			p_bs->Serialize(p_writeToBitstream, Action.StrafePower);
			p_bs->Serialize(p_writeToBitstream, Action.Jump);
			for (int i = 0; i < 2; ++i)
				p_bs->Serialize(p_writeToBitstream, Action.Angle[i]);
			p_bs->Serialize(p_writeToBitstream, Action.ActivateAbility);
			p_bs->Serialize(p_writeToBitstream, Action.SelectedAbility);
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
			p_bs->Serialize(p_writeToBitstream, CurrentPlayers);
			p_bs->Serialize(p_writeToBitstream, MaxPlayers);
			p_bs->Serialize(p_writeToBitstream, PasswordProtected);
			p_bs->Serialize(p_writeToBitstream, IsDedicated);
			p_bs->Serialize(p_writeToBitstream, GameMode);
			p_bs->Serialize(p_writeToBitstream, MatchTimeSeconds);
			p_bs->Serialize(p_writeToBitstream, KillCount);
		}
	}
}

#endif
