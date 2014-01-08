#include "MatchStateSystem.h"

namespace RootForce
{


	void MatchStateSystem::Process()
	{
		//State: team death match
		//loop through all members of a team, adding their score together.
		//loop second team

		//State: free for all
		//Not to be implemented yet ;) 
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
		if(p_team == 1)
			return m_world->GetEntityManager()->GetComponent<TDMRuleSet>( m_world->GetTagManager()->GetEntityByTag("MatchState") )->TeamOneScore;
		else if(p_team == 2)
			return m_world->GetEntityManager()->GetComponent<TDMRuleSet>( m_world->GetTagManager()->GetEntityByTag("MatchState") )->TeamTwoScore;
	}

	bool MatchStateSystem::IsMatchOver()
	{
		if(m_world->GetEntityManager()->GetComponent<TDMRuleSet>( m_world->GetTagManager()->GetEntityByTag("MatchState") )->TeamOneScore >=
			m_world->GetEntityManager()->GetComponent<TDMRuleSet>( m_world->GetTagManager()->GetEntityByTag("MatchState") )->ScoreLimit || 
			m_world->GetEntityManager()->GetComponent<TDMRuleSet>( m_world->GetTagManager()->GetEntityByTag("MatchState") )->TeamTwoScore >=
			m_world->GetEntityManager()->GetComponent<TDMRuleSet>( m_world->GetTagManager()->GetEntityByTag("MatchState") )->ScoreLimit)
			return true;
		else if(m_world->GetEntityManager()->GetComponent<TDMRuleSet>( m_world->GetTagManager()->GetEntityByTag("MatchState") )->TimeLeft >= 0)
			return true;
		else
			return false;
	}

	

}