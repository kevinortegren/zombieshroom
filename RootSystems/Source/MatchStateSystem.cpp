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
	
	struct Score
	{
		int Team;
		std::string Name;
		int Kills;
		int Deaths;

		Score(int p_team, std::string p_name, int p_kills, int p_deaths)
			: Team(p_team), Name(p_name), Kills(p_kills), Deaths(p_deaths)
		{}
	};
	bool CompareScores(Score &p_score1, Score &p_score2)
	{
		if(p_score1.Kills > p_score2.Kills) // If the score is higher
			return true;
		else if(p_score1.Kills == p_score2.Kills && p_score1.Deaths < p_score2.Deaths) // Or if equal, but less deaths
			return true;
		return false;
	};
	std::string MatchStateSystem::GetScoreList()
	{
		std::vector<Score> scoreList;
		std::string scoreString = "[";
		for(auto pair : g_networkEntityMap)
		{
			if(pair.first.ActionID != Network::ReservedActionID::CONNECT || pair.first.SequenceID != 0 || !pair.second)
				continue;

			PlayerComponent* playerComponent = g_world->GetEntityManager()->GetComponent<PlayerComponent>(pair.second);
			scoreList.push_back(Score(playerComponent->TeamID, playerComponent->Name, playerComponent->Score, playerComponent->Deaths));
		}
		std::sort(scoreList.begin(), scoreList.end(), &CompareScores);
		for(auto score : scoreList)
		{
			scoreString += "["+std::to_string(score.Team)+",'" + score.Name + "',"+std::to_string(score.Kills)+","+std::to_string(score.Deaths)+"],";
		}
		scoreString += "]";
		return scoreString;
	}

	bool MatchStateSystem::IsMatchOver()
	{
		ECS::Entity* matchState = m_world->GetTagManager()->GetEntityByTag("MatchState");
		TDMRuleSet* ruleSet = m_world->GetEntityManager()->GetComponent<TDMRuleSet>( matchState );
		if(ruleSet->ScoreLimit > 0
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