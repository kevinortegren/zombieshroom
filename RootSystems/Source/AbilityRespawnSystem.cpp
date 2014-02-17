#ifndef COMPILE_LEVEL_EDITOR
#include "AbilityRespawnSystem.h"
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootEngine/Script/Include/RootScript.h>
#include <RootSystems/Include/Script.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootSystems/Include/CollisionSystem.h>
#include <RootEngine/Physics/Include/RootPhysics.h>
#include <RakNet/GetTime.h>
#include <fstream>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/BitStream.h>

extern RootForce::Network::NetworkEntityMap g_networkEntityMap;

namespace RootForce
{
	void AbilityRespawnSystem::Init()
	{
		m_respawn.Init(m_world->GetEntityManager());
		m_transform.Init(m_world->GetEntityManager());
		m_network.Init(m_world->GetEntityManager());
		m_script.Init(m_world->GetEntityManager());
	}

	void AbilityRespawnSystem::ProcessEntity( ECS::Entity* p_entity )
	{
		float dt = m_world->GetDelta();
		AbilityRespawnComponent* respawn = m_respawn.Get(p_entity);
		Transform* transform = m_transform.Get(p_entity);
		Script* script = m_script.Get(p_entity);
		Network::NetworkComponent* network = m_network.Get(p_entity);

		if(respawn->Claimed != Network::ReservedUserID::NONE && respawn->CurrentAbility.Name.compare("") != 0)
		{
			if(m_serverPeer != nullptr)
			{
				//send message:  Ability claimed
				RootForce::NetworkMessage::AbilityClaimedBy m;
				m.User = respawn->Claimed;
				m.AbilitySpawnPointID = network->ID;

				RakNet::BitStream bs;
				bs.Write((RakNet::MessageID) ID_TIMESTAMP);
				bs.Write(RakNet::GetTime());
				bs.Write((RakNet::MessageID) RootForce::NetworkMessage::MessageType::AbilityClaimedBy);
				m.Serialize(true, &bs);

				m_serverPeer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
			}
			
			//Give the ability to the correct player
			RootForce::Network::NetworkEntityID id;
			id.UserID = respawn->Claimed;
			id.ActionID = RootForce::Network::ReservedActionID::CONNECT;
			id.SequenceID = 0;
			PlayerComponent* player = m_world->GetEntityManager()->GetComponent<PlayerComponent>(g_networkEntityMap[id]);
			player->AbilityScripts[player->SelectedAbility] = respawn->CurrentAbility;

			//Clear the ability spawnpoint
			respawn->CurrentAbility = AbilityInfo(); //Make an empty abilityInfo
			Collision* collision = m_world->GetEntityManager()->GetComponent<Collision>(p_entity);
			m_engineContext->m_physics->RemoveObject(*collision->m_handle);
			m_world->GetEntityManager()->RemoveComponent<Renderable>(p_entity); //Remove the render component to show that there is no ability to claim
			m_world->GetEntityManager()->RemoveComponent<CollisionResponder>(p_entity);
			m_world->GetEntityManager()->RemoveComponent<Collision>(p_entity);
			
			respawn->Timer = 30.0f; //30 second timer until a new ability respawns
			respawn->Claimed = Network::ReservedUserID::NONE;
		}
		else if(respawn->Timer <= 0.0f && respawn->CurrentAbility.Name.compare("") == 0)
		{
			unsigned chosenSpawn = rand()%m_levelAbilities.size();
			respawn->CurrentAbility.Name = m_levelAbilities.at(chosenSpawn);
			respawn->CurrentAbility.Charges = (int) m_engineContext->m_script->GetGlobalNumber("charges", respawn->CurrentAbility.Name);
			respawn->CurrentAbility.OnCooldown = false;
			respawn->Claimed = Network::ReservedUserID::NONE;

			Renderable* renderable = m_world->GetEntityManager()->CreateComponent<Renderable>(p_entity);
			renderable->m_model = m_engineContext->m_resourceManager->LoadCollada("AbilitySpawnPoint");
			renderable->m_material = m_engineContext->m_renderer->CreateMaterial(respawn->CurrentAbility.Name);
			renderable->m_material->m_textures[Render::TextureSemantic::DIFFUSE] = m_engineContext->m_resourceManager->LoadTexture(respawn->CurrentAbility.Name, Render::TextureType::TEXTURE_2D);
			renderable->m_material->m_effect = m_engineContext->m_resourceManager->GetEffect("Mesh");

			CollisionResponder* collisionResp = m_world->GetEntityManager()->CreateComponent<CollisionResponder>(p_entity);
			Collision* collision = m_world->GetEntityManager()->CreateComponent<Collision>(p_entity);
			collision->m_handle = m_engineContext->m_physics->CreateHandle((void*)p_entity, RootEngine::Physics::PhysicsType::TYPE_ABILITYSPAWN, true);
			collision->m_meshHandle = "AbilitySpawnPoint";
			m_engineContext->m_physics->BindSphereShape(*collision->m_handle, transform->m_position , glm::quat(0,0,0,1), 1.0f, 1.0f, false);
			m_engineContext->m_physics->SetCollisionContainer(*collision->m_handle, &collisionResp->m_collisions);

		}
		respawn->Timer -= dt;
	}

	void AbilityRespawnSystem::LoadAbilities(std::string p_abilityPack)
	{
		std::ifstream file(m_workingDir + "Assets/AbilityPacks/" + p_abilityPack + ".txt");
		if(file.is_open())
		{
			std::string temp;
			while(std::getline(file, temp))
			{
				m_levelAbilities.push_back(temp);
			}
			m_engineContext->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Succesfully loaded the ability pack");
		}
		else
			m_engineContext->m_logger->LogText(LogTag::GAME, LogLevel::NON_FATAL_ERROR, "Ability pack not found");
	}

	void AbilityRespawnSystem::AttatchComponentToPoints()
	{
		ECS::GroupManager::GroupRange points = m_world->GetGroupManager()->GetEntitiesInGroup("AbilitySpawnPoint");

		for(std::multimap<std::string, ECS::Entity*>::iterator itr = points.first; itr != points.second; ++itr)
		{
			m_world->GetEntityManager()->CreateComponent<AbilityRespawnComponent>(itr->second);
			m_world->GetEntityManager()->CreateComponent<Network::NetworkComponent>(itr->second);
			m_world->GetEntityManager()->CreateComponent<Script>(itr->second);

			Network::NetworkComponent* network = m_world->GetEntityManager()->GetComponent<Network::NetworkComponent>(itr->second);
			network->SetID(itr->second,Network::ReservedUserID::NONE, Network::ReservedActionID::ABILITYSPAWN);
			Script* script = m_world->GetEntityManager()->GetComponent<Script>(itr->second);
			script->Name = "AbilitySpawnPoint";

			AbilityRespawnComponent* respawn = m_world->GetEntityManager()->GetComponent<AbilityRespawnComponent>(itr->second);
			Transform* transform = m_world->GetEntityManager()->GetComponent<Transform>(itr->second);
			
			unsigned chosenSpawn = rand()%m_levelAbilities.size();
			respawn->CurrentAbility.Name = m_levelAbilities.at(chosenSpawn);
			respawn->CurrentAbility.Charges = (int) m_engineContext->m_script->GetGlobalNumber("charges", respawn->CurrentAbility.Name);
			respawn->CurrentAbility.OnCooldown = false;

			Renderable* renderable = m_world->GetEntityManager()->CreateComponent<Renderable>(itr->second);
			renderable->m_model = m_engineContext->m_resourceManager->LoadCollada("AbilitySpawnPoint");
			renderable->m_material = m_engineContext->m_renderer->CreateMaterial(respawn->CurrentAbility.Name);
			renderable->m_material->m_textures[Render::TextureSemantic::DIFFUSE] = m_engineContext->m_resourceManager->LoadTexture(respawn->CurrentAbility.Name, Render::TextureType::TEXTURE_2D);
			renderable->m_material->m_effect = m_engineContext->m_resourceManager->GetEffect("Mesh");

			CollisionResponder* collisionResp = m_world->GetEntityManager()->CreateComponent<CollisionResponder>(itr->second);
			Collision* collision = m_world->GetEntityManager()->CreateComponent<Collision>(itr->second);
			collision->m_handle = m_engineContext->m_physics->CreateHandle((void*)itr->second, RootEngine::Physics::PhysicsType::TYPE_ABILITYSPAWN, true);
			collision->m_meshHandle = "AbilitySpawnPoint";
			m_engineContext->m_physics->BindSphereShape(*collision->m_handle, transform->m_position , glm::quat(0,0,0,1), 1.0f, 1.0f, false);
			m_engineContext->m_physics->SetCollisionContainer(*collision->m_handle, &collisionResp->m_collisions);
		}
	}


}

#endif