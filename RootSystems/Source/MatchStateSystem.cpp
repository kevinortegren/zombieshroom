#ifndef COMPILE_LEVEL_EDITOR

#include "MatchStateSystem.h"
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootForce/Include/GameStates.h>

extern RootForce::Network::NetworkEntityMap g_networkEntityMap;

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
		Network::NetworkEntityID killerNetworkID;
		killerNetworkID.UserID = p_killerID;
		killerNetworkID.ActionID = Network::ReservedActionID::CONNECT;
		killerNetworkID.SequenceID = 0;

		Network::NetworkEntityID deadNetworkID;
		deadNetworkID.UserID = p_deadID;
		deadNetworkID.ActionID = Network::ReservedActionID::CONNECT;
		deadNetworkID.SequenceID = 0;

		m_world->GetEntityManager()->GetComponent<PlayerComponent>(g_networkEntityMap[killerNetworkID])->Score ++;
		m_world->GetEntityManager()->GetComponent<PlayerComponent>(g_networkEntityMap[deadNetworkID])->Score ++;
	}

	

}

#endif