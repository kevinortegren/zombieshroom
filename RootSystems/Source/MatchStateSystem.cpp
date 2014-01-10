#include "MatchStateSystem.h"
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/Network/NetworkEntityMap.h>
#include <RootForce/Include/GameStates.h>
namespace RootForce
{


	void MatchStateSystem::Process()
	{
		if(m_world->GetEntityManager()->GetComponent<HealthComponent>(m_world->GetTagManager()->GetEntityByTag("Player"))->Health >= 0 &&
			m_world->GetEntityManager()->GetComponent<HealthComponent>(m_world->GetTagManager()->GetEntityByTag("Player"))->IsDead == false)
		{
			m_world->GetEntityManager()->GetComponent<HealthComponent>(m_world->GetTagManager()->GetEntityByTag("Player"))->IsDead = true;
			//Send a message over the network that a player has been killed and who by whom he was killed
		}

		if(m_world->GetEntityManager()->GetComponent<HealthComponent>(m_world->GetTagManager()->GetEntityByTag("Player"))->WantsRespawn)
		{
			m_world->GetEntityManager()->GetComponent<HealthComponent>(m_world->GetTagManager()->GetEntityByTag("Player"))->WantsRespawn = false;
			m_world->GetEntityManager()->GetComponent<HealthComponent>(m_world->GetTagManager()->GetEntityByTag("Player"))->Health = 100;
			//Respawn player at random spawnpoint
		}
	}

	void MatchStateSystem::UpdateDeltatime( float p_deltaTime )
	{
		m_world->GetEntityManager()->GetComponent<TDMRuleSet>( m_world->GetTagManager()->GetEntityByTag("MatchState") )->TimeLeft -= p_deltaTime;
	}

	void MatchStateSystem::SetLoggingInterface( Logging* p_logger )
	{
		m_logger = p_logger;
	}

	float MatchStateSystem::GetTimeLeft()
	{
		return m_world->GetEntityManager()->GetComponent<TDMRuleSet>( m_world->GetTagManager()->GetEntityByTag("MatchState") )->TimeLeft;
	}

	int MatchStateSystem::GetTeamScore( int p_team )
	{
			return m_world->GetEntityManager()->GetComponent<TDMRuleSet>( m_world->GetTagManager()->GetEntityByTag("MatchState") )->TeamScore[p_team];
	}

	bool MatchStateSystem::IsMatchOver()
	{
		if(m_world->GetEntityManager()->GetComponent<TDMRuleSet>( m_world->GetTagManager()->GetEntityByTag("MatchState") )->TeamScore[1] >=
			m_world->GetEntityManager()->GetComponent<TDMRuleSet>( m_world->GetTagManager()->GetEntityByTag("MatchState") )->ScoreLimit || 
			m_world->GetEntityManager()->GetComponent<TDMRuleSet>( m_world->GetTagManager()->GetEntityByTag("MatchState") )->TeamScore[2] >=
			m_world->GetEntityManager()->GetComponent<TDMRuleSet>( m_world->GetTagManager()->GetEntityByTag("MatchState") )->ScoreLimit)
			return true;
		else if(m_world->GetEntityManager()->GetComponent<TDMRuleSet>( m_world->GetTagManager()->GetEntityByTag("MatchState") )->TimeLeft >= 0)
			return true;
		else
			return false;
	}

	void MatchStateSystem::AwardPlayerKill( int p_killerID, int p_deadID )
	{
		m_world->GetEntityManager()->GetComponent<ScoreComponent>(m_networkContext->m_networkEntityMap->GetPlayerEntityFromUserID(p_killerID))->Score ++;

		m_world->GetEntityManager()->GetComponent<ScoreComponent>(m_networkContext->m_networkEntityMap->GetPlayerEntityFromUserID(p_deadID))->Deaths ++;
	}

	

}