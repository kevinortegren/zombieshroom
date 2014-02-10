#ifndef COMPILE_LEVEL_EDITOR
#include "AbilityRespawnSystem.h"
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/RenderingSystem.h>
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
			//TODO: read charge count and texture from script

			Renderable* renderable = m_world->GetEntityManager()->CreateComponent<Renderable>(p_entity);
			//TODO: read texture and material from script
		}
		respawn->Timer -= dt;
	}

	void AbilityRespawnSystem::LoadAbilities(std::string p_abilityPack)
	{
		//TODO get the ability-pack from the resource manager
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

}

#endif