#include "GameLogicSystem.h"

namespace RootForce
{


	void GameLogicSystem::Process()
	{
		//State: team death match
		//loop through all members of a team, adding their score together.
		//If it equals or exceeeds the score limit of rule entity, set match over equal to true
		//loop second team
		//withdraw deltatime from rule entity's TimeLeft

		//State: free for all
		//Not to be implemented yet ;) 
	}

	void GameLogicSystem::SetLoggingInterface( Logging* p_logger )
	{
		m_logger = p_logger;
	}

	double GameLogicSystem::GetTimeLeft()
	{
		return m_world->GetEntityManager()->GetComponent<TDMRuleSet>( m_world->GetTagManager()->GetEntityByTag("Player") )->TimeLeft;
	}

	int GameLogicSystem::GetTeamScore( int p_team )
	{
		if(p_team == 1)
			return m_world->GetEntityManager()->GetComponent<TDMRuleSet>( m_world->GetTagManager()->GetEntityByTag("Player") )->TeamOneScore;
		else if(p_team == 2)
			return m_world->GetEntityManager()->GetComponent<TDMRuleSet>( m_world->GetTagManager()->GetEntityByTag("Player") )->TeamTwoScore;
	}

	bool GameLogicSystem::IsMatchOver()
	{
		if(m_world->GetEntityManager()->GetComponent<TDMRuleSet>( m_world->GetTagManager()->GetEntityByTag("Player") )->TeamOneScore >=
			m_world->GetEntityManager()->GetComponent<TDMRuleSet>( m_world->GetTagManager()->GetEntityByTag("Player") )->ScoreLimit || 
			m_world->GetEntityManager()->GetComponent<TDMRuleSet>( m_world->GetTagManager()->GetEntityByTag("Player") )->TeamTwoScore >=
			m_world->GetEntityManager()->GetComponent<TDMRuleSet>( m_world->GetTagManager()->GetEntityByTag("Player") )->ScoreLimit)
			return true;
		else
			return false;
	}

}