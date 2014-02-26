#ifndef COMPILE_LEVEL_EDITOR
#include "AbilitySpawnSystem.h"
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
#include <RootSystems/Include/LightSystem.h>

extern RootForce::Network::NetworkEntityMap g_networkEntityMap;

namespace RootForce
{
	void AbilitySpawnSystem::Init()
	{
		m_respawn.Init(m_world->GetEntityManager());
		m_transform.Init(m_world->GetEntityManager());
		m_network.Init(m_world->GetEntityManager());
		m_script.Init(m_world->GetEntityManager());
		m_particle.Init(m_world->GetEntityManager());
	}

	void AbilitySpawnSystem::ProcessEntity( ECS::Entity* p_entity )
	{
		float dt = m_world->GetDelta();
		AbilitySpawnComponent* respawn = m_respawn.Get(p_entity);
		Network::NetworkComponent* network = m_network.Get(p_entity);
		Transform* transform = m_transform.Get(p_entity);

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
			PlayerComponent* player = m_world->GetEntityManager()->GetComponent<PlayerComponent>(RootForce::Network::FindEntity(g_networkEntityMap, id));
			player->AbilityScripts[player->SelectedAbility] = respawn->CurrentAbility;

			//Remove the collision and renderable component as well as clean out the current ability
			HideSpawnpoint(p_entity);
			
		}
		else if(respawn->Timer <= 0.0f && respawn->CurrentAbility.Name.compare("") == 0)
		{
			
			//Add new components to the spawnpoint and give it a new current ability
			if(respawn->AbilityReceived.compare("") == 0)
				NewServerAbility(p_entity); //We have not received a new ability yet
			else
				RevealSpawnpoint(p_entity); //We have received the ability and can now reveal the point

		}
		respawn->Timer -= dt;
		transform->m_orientation.Rotate(0 , dt * 50.0f , 0);

		//Ugly fix which makes sure that clients that join the server get render and collision components on their spawn points, should only happen once
		Renderable* render = m_world->GetEntityManager()->GetComponent<Renderable>(p_entity);
		if(respawn->CurrentAbility.Name.compare("") != 0 && render == nullptr)
		{
			CreateRenderComponent(p_entity);
			CreateCollisionComponents(p_entity);
		}
	}

	void AbilitySpawnSystem::LoadAbilities(std::string p_abilityPack)
	{
		std::ifstream file(m_workingDir + "Assets/AbilityPacks/" + p_abilityPack + ".txt");
		if(file.is_open())
		{
			std::string temp;
			while(std::getline(file, temp))
			{
				m_levelAbilities.push_back(temp);
				m_engineContext->m_resourceManager->LoadScript(temp);
			}
			m_engineContext->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Succesfully loaded the ability pack");
		}
		else
			m_engineContext->m_logger->LogText(LogTag::GAME, LogLevel::NON_FATAL_ERROR, "Ability pack not found");
	}

	void AbilitySpawnSystem::AttachComponentToPoints()
	{
		ECS::GroupManager::GroupRange points = m_world->GetGroupManager()->GetEntitiesInGroup("AbilitySpawnPoint"); //Get all the entities that are Ability spawn points

		for(std::multimap<std::string, ECS::Entity*>::iterator itr = points.first; itr != points.second; ++itr)
		{
			m_world->GetEntityManager()->CreateComponent<AbilitySpawnComponent>(itr->second);
			m_world->GetEntityManager()->CreateComponent<Network::NetworkComponent>(itr->second);
			m_world->GetEntityManager()->CreateComponent<Script>(itr->second);

			Network::NetworkComponent* network = m_world->GetEntityManager()->GetComponent<Network::NetworkComponent>(itr->second);
			network->SetID(itr->second,Network::ReservedUserID::NONE, Network::ReservedActionID::ABILITYSPAWN);
			Script* script = m_world->GetEntityManager()->GetComponent<Script>(itr->second);
			script->Name = "AbilitySpawnPoint";

		}
	}

	void AbilitySpawnSystem::HideSpawnpoint( ECS::Entity* p_entity )
	{
		AbilitySpawnComponent* respawn = m_respawn.Get(p_entity);

		respawn->CurrentAbility = AbilityInfo(); //Make an empty abilityInfo

		Collision* collision = m_world->GetEntityManager()->GetComponent<Collision>(p_entity);

		//m_engineContext->m_physics->RemoveObject(*collision->m_handle); //Remove the object from the physics

		//Remove all components used for rendering and collision
		m_world->GetEntityManager()->RemoveComponent<Renderable>(p_entity);
		m_world->GetEntityManager()->RemoveComponent<PointLight>(p_entity);
		m_world->GetEntityManager()->RemoveComponent<CollisionResponder>(p_entity);
		m_world->GetEntityManager()->RemoveComponent<Collision>(p_entity);

		respawn->Timer = 30.0f; //30 second timer until a new ability respawns
		respawn->Claimed = Network::ReservedUserID::NONE; //no current claimiant
	}

	void AbilitySpawnSystem::RevealSpawnpoint( ECS::Entity* p_entity )
	{
		
		SetClientCurrentAbility(p_entity);
		
		CreateRenderComponent(p_entity);

		CreateCollisionComponents(p_entity);

	}


	void AbilitySpawnSystem::NewServerAbility( ECS::Entity* p_entity )
	{
		//Only do this if we are a server
		if(m_serverPeer != nullptr)
		{
			Network::NetworkComponent* network = m_network.Get(p_entity);

			//Get a new random ability from the pack
			unsigned chosenSpawn = rand()%m_levelAbilities.size();

			RootForce::NetworkMessage::AbilitySpawn m;
			m.ID = network->ID;
			m.AbilityName = RakNet::RakString(m_levelAbilities.at(chosenSpawn).c_str());

			RakNet::BitStream bs;
			bs.Write((RakNet::MessageID) ID_TIMESTAMP);
			bs.Write(RakNet::GetTime());
			bs.Write((RakNet::MessageID) RootForce::NetworkMessage::MessageType::AbilitySpawn);
			m.Serialize(true, &bs);

			//Send it to all the clients
			m_serverPeer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
		}
	}


	void AbilitySpawnSystem::SetClientCurrentAbility( ECS::Entity* p_entity )
	{
		//Only set if we are a client
		if(m_clientPeer != nullptr)
		{
			AbilitySpawnComponent* respawn = m_respawn.Get(p_entity);

			respawn->CurrentAbility.Name = respawn->AbilityReceived;
			respawn->CurrentAbility.Charges = (int) m_engineContext->m_script->GetGlobalNumber("charges", respawn->CurrentAbility.Name);
			respawn->CurrentAbility.OnCooldown = false;
			respawn->Claimed = Network::ReservedUserID::NONE;
			respawn->AbilityReceived = "";
		}
	}


	void AbilitySpawnSystem::CreateRenderComponent(ECS::Entity* p_entity)
	{
		AbilitySpawnComponent* respawn = m_respawn.Get(p_entity);

		Renderable* renderable = m_world->GetEntityManager()->CreateComponent<Renderable>(p_entity);
		renderable->m_model = m_engineContext->m_resourceManager->LoadCollada("AbilitySpawnPoint");
		renderable->m_material = m_engineContext->m_renderer->CreateMaterial(respawn->CurrentAbility.Name);
		renderable->m_material->m_textures[Render::TextureSemantic::GLOW] = m_engineContext->m_resourceManager->LoadTexture(respawn->CurrentAbility.Name, Render::TextureType::TEXTURE_2D);
		renderable->m_material->m_textures[Render::TextureSemantic::DIFFUSE] = m_engineContext->m_resourceManager->LoadTexture("AbilityBackground", Render::TextureType::TEXTURE_2D);
		renderable->m_material->m_effect = m_engineContext->m_resourceManager->LoadEffect("Mesh");

		PointLight* pl = m_world->GetEntityManager()->CreateComponent<PointLight>(p_entity);
		pl->m_color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		pl->m_attenuation = glm::vec3(0.0f, 0.3f, 0.0f);

	}

	void AbilitySpawnSystem::CreateCollisionComponents(ECS::Entity* p_entity)
	{
		Transform* transform = m_transform.Get(p_entity);

		CollisionResponder* collisionResp = m_world->GetEntityManager()->CreateComponent<CollisionResponder>(p_entity);
		Collision* collision = m_world->GetEntityManager()->CreateComponent<Collision>(p_entity);
		collision->m_handle = m_engineContext->m_physics->CreateHandle((void*)p_entity, RootEngine::Physics::PhysicsType::TYPE_ABILITYSPAWN, true);
		collision->m_meshHandle = "AbilitySpawnPoint";
		m_engineContext->m_physics->BindSphereShape(*collision->m_handle, transform->m_position , glm::quat(0,0,0,1), 1.0f, 1.0f, false);
		m_engineContext->m_physics->SetCollisionContainer(*collision->m_handle, &collisionResp->m_collisions);
	}

	void AbilitySpawnSystem::CreateParticleEmitter( ECS::Entity* p_entity )
	{
		ParticleEmitter* emitter = m_world->GetEntityManager()->CreateComponent<ParticleEmitter>(p_entity);

		emitter->m_particleSystems = g_engineContext.m_resourceManager->LoadParticleEmitter("AbilitySpawn", false);

		for(unsigned i = 0; i < emitter->m_particleSystems.size(); i++)
			emitter->m_systems.push_back(g_engineContext.m_renderer->CreateParticleSystem());
	}

}

#endif