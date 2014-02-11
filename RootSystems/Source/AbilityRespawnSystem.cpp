#ifndef COMPILE_LEVEL_EDITOR
#include "AbilityRespawnSystem.h"
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootEngine/Script/Include/RootScript.h>
#include <RootSystems/Include/Script.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <fstream>

namespace RootForce
{
	void AbilityRespawnSystem::Init()
	{
		m_respawn.Init(m_world->GetEntityManager());
	}

	void AbilityRespawnSystem::ProcessEntity( ECS::Entity* p_entity )
	{
		float dt = m_world->GetDelta();
		AbilityRespawnComponent* respawn = m_respawn.Get(p_entity);

		if(respawn->Claimed && !respawn->CurrentAbility.Name.compare(""))
		{
			respawn->CurrentAbility = AbilityInfo(); //Make an empty abilityInfo
			m_world->GetEntityManager()->RemoveComponent<Renderable>(p_entity); //Remove the render component to show that there is no ability to claim
			respawn->Timer = 30; //30 second timer until a new ability respawns
		}
		else if(respawn->Timer <= 0 && respawn->CurrentAbility.Name.compare(""))
		{
			unsigned chosenSpawn = rand()%m_levelAbilities.size()-1;
			respawn->CurrentAbility.Name = m_levelAbilities.at(chosenSpawn);
			respawn->CurrentAbility.Charges = (int) m_engineContext->m_script->GetGlobalNumber("charges", respawn->CurrentAbility.Name);
			respawn->CurrentAbility.OnCooldown = false;

			Renderable* renderable = m_world->GetEntityManager()->CreateComponent<Renderable>(p_entity);
			renderable->m_model = m_engineContext->m_resourceManager->LoadCollada("AbilityRespawnPoint");
			renderable->m_material = m_engineContext->m_renderer->CreateMaterial(respawn->CurrentAbility.Name);
			renderable->m_material->m_textures[Render::TextureSemantic::DIFFUSE] = m_engineContext->m_resourceManager->GetTexture(respawn->CurrentAbility.Name);
			renderable->m_material->m_effect = m_engineContext->m_resourceManager->GetEffect("Mesh");
		}
		respawn->Timer -= dt;
	}

	void AbilityRespawnSystem::LoadAbilities(std::string p_abilityPack)
	{
		std::ifstream file(p_abilityPack);
		if(file.is_open())
		{
			std::string temp;
			while(std::getline(file, temp))
			{
				m_levelAbilities.push_back(temp);
			}
		}
		else
			m_engineContext->m_logger->LogText(LogTag::GAME, LogLevel::NON_FATAL_ERROR, "Ability pack not found");
	}

	void AbilityRespawnSystem::AttatchComponentToPoints()
	{
		ECS::GroupManager::GroupRange points = m_world->GetGroupManager()->GetEntitiesInGroup("AbilitySpawnPoints");

		for(std::multimap<std::string, ECS::Entity*>::iterator itr = points.first; itr != points.second; ++itr)
		{
			m_world->GetEntityManager()->CreateComponent<AbilityRespawnComponent>(itr->second);
		}
	}

}

#endif