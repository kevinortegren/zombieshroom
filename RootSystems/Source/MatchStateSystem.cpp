#ifndef COMPILE_LEVEL_EDITOR

#include "MatchStateSystem.h"
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootForce/Include/GameStates.h>

extern RootForce::Network::NetworkEntityMap g_networkEntityMap;
extern ECS::World* g_world;

namespace RootForce
{
	void MatchStateSystem::Process()
	{
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
		ECS::Entity* matchState = m_world->GetTagManager()->GetEntityByTag("MatchState");
		TDMRuleSet* ruleSet = m_world->GetEntityManager()->GetComponent<TDMRuleSet>( matchState );
		if(ruleSet->ScoreLimit <= 0
			&& (ruleSet->TeamScore[1] >= ruleSet->ScoreLimit || 
			ruleSet->TeamScore[2] >= ruleSet->ScoreLimit))
			return true;
		else if(ruleSet->TimeLeft <= 0)
			return true;
		else
			return false;
	}

	void MatchStateSystem::AwardPlayerKill( RootForce::Network::UserID_t p_killerID, RootForce::Network::UserID_t p_deadID )
	{
		Network::NetworkEntityID killerNetworkID;
		killerNetworkID.UserID = p_killerID;
		killerNetworkID.ActionID = Network::ReservedActionID::CONNECT;
		killerNetworkID.SequenceID = 0;

		Network::NetworkEntityID deadNetworkID;
		deadNetworkID.UserID = p_deadID;
		deadNetworkID.ActionID = Network::ReservedActionID::CONNECT;
		deadNetworkID.SequenceID = 0;
		
		// Award score for killer team
		if(p_killerID != Network::ReservedUserID::NONE)
		{
			ECS::Entity* matchStateEntity = g_world->GetTagManager()->GetEntityByTag("MatchState");
			RootForce::TDMRuleSet* rules = g_world->GetEntityManager()->GetComponent<RootForce::TDMRuleSet>(matchStateEntity);
			PlayerComponent* killerPlayerComponent = g_world->GetEntityManager()->GetComponent<PlayerComponent>(g_networkEntityMap[killerNetworkID]);

			rules->TeamScore[killerPlayerComponent->TeamID]++;
			killerPlayerComponent->Score++;
		}
		g_world->GetEntityManager()->GetComponent<PlayerComponent>(g_networkEntityMap[deadNetworkID])->Deaths ++;
	}

	

}

#endif