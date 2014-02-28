#ifndef COMPILE_LEVEL_EDITOR

#include "MatchStateSystem.h"
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootForce/Include/GameStates.h>
#include <RootEngine/GUI/Include/WebView.h>
#include <RootEngine/Script/Include/RootScript.h>

extern RootForce::Network::NetworkEntityMap g_networkEntityMap;
extern ECS::World* g_world;

namespace RootForce
{
	void MatchStateSystem::Process()
	{
		int numPlayers = 0;
		ECS::Entity* matchState = m_world->GetTagManager()->GetEntityByTag("MatchState");
		TDMRuleSet* ruleSet = m_world->GetEntityManager()->GetComponent<TDMRuleSet>( matchState );
		for(auto pair : g_networkEntityMap)
		{
			if(pair.first.ActionID != Network::ReservedActionID::CONNECT
				|| pair.first.SequenceID != RootForce::Network::SEQUENCE_PLAYER_ENTITY
				|| !pair.second)
				continue;

			PlayerComponent* playerComponent = g_world->GetEntityManager()->GetComponent<PlayerComponent>(pair.second);
			if(playerComponent->TeamID != 0)
				numPlayers++;
		}
		switch(ruleSet->CurrentState)
		{
			case MatchState::Warmup:
				m_hud->GetView()->BufferJavascript("Announce('Waiting for players...', -1);");
				if(numPlayers >= ruleSet->MinPlayers)
				{
					ruleSet->CountDown = 5.0f;
					ruleSet->CurrentState = MatchState::CountDown;
					m_hud->GetView()->BufferJavascript("Announce('5',1);Announce('4',1);Announce('3',1);Announce('2',1);Announce('1',1);");
				}
				break;
			case MatchState::CountDown:
				if(ruleSet->CountDown <= 0.0f)
				{
					ruleSet->TeamScore[1] = 0;
					ruleSet->TeamScore[2] = 0;
					m_abilitySpawnSystem->ResetAllPoints();
					// Make a copy of entity vector to avoid iterator issues on deletion
					RootForce::Network::NetworkEntityMap entities = g_networkEntityMap;
					for(auto pair : entities)
					{
						if(!pair.second)
							continue;
						if(pair.first.ActionID == Network::ReservedActionID::CONNECT
							&& pair.first.SequenceID == RootForce::Network::SEQUENCE_PLAYER_ENTITY)
						{
							PlayerComponent* playerComponent = g_world->GetEntityManager()->GetComponent<PlayerComponent>(pair.second);
							playerComponent->Score = 0;
							playerComponent->Deaths = 0;

							if(playerComponent->TeamID != 0)
							{
								HealthComponent* health = g_world->GetEntityManager()->GetComponent<HealthComponent>(pair.second);
								health->Health = 0;
								health->IsDead = true;
								health->WantsRespawn = true;
								health->RespawnDelay = 0.0f;
							}
						}
						// If the entity has a script and it is neither Player or AbilitySpawnPoint, assume it's an ability and attempt to remove it
						else if(g_world->GetEntityManager()->GetComponent<Script>(pair.second)
							&& g_world->GetEntityManager()->GetComponent<Script>(pair.second)->Name.compare("Player") != 0
							&& g_world->GetEntityManager()->GetComponent<Script>(pair.second)->Name.compare("AbilitySpawnPoint") != 0
							&& g_world->GetEntityManager()->GetComponent<Script>(pair.second)->Name.compare("MatchState") != 0)
						{
							Script* script = g_world->GetEntityManager()->GetComponent<Script>(pair.second);
							g_engineContext.m_script->SetFunction(script->Name, "OnDestroy");
							g_engineContext.m_script->AddParameterUserData(pair.second, sizeof(ECS::Entity*), "Entity");
							g_engineContext.m_script->ExecuteScript();
							//g_world->GetEntityManager()->RemoveEntity(pair.second);
						}
					}
					ruleSet->CurrentState = MatchState::Match;
					m_hud->GetView()->BufferJavascript("Announce('May the roots be with you!',3);");
				}
				break;
			case MatchState::Match:
				//Nothing, and you will never be nothing!
				break;
		}
	}

	void MatchStateSystem::UpdateDeltatime( float p_deltaTime )
	{
		ECS::Entity* matchState = m_world->GetTagManager()->GetEntityByTag("MatchState");
		TDMRuleSet* ruleSet = m_world->GetEntityManager()->GetComponent<TDMRuleSet>( matchState );

		if(ruleSet->CurrentState == MatchState::Match)
			m_world->GetEntityManager()->GetComponent<TDMRuleSet>( m_world->GetTagManager()->GetEntityByTag("MatchState") )->TimeLeft -= p_deltaTime;
		else if(ruleSet->CurrentState == MatchState::CountDown)
			ruleSet->CountDown -= p_deltaTime;
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
			if(pair.first.ActionID != Network::ReservedActionID::CONNECT || pair.first.SequenceID != RootForce::Network::SEQUENCE_PLAYER_ENTITY || !pair.second)
				continue;

			PlayerComponent* playerComponent = g_world->GetEntityManager()->GetComponent<PlayerComponent>(pair.second);
			scoreList.push_back(Score(playerComponent->TeamID, playerComponent->Name, playerComponent->Score, playerComponent->Deaths));
		}
		std::sort(scoreList.begin(), scoreList.end(), &CompareScores);
		bool first = true;
		for(auto score : scoreList)
		{
			if(first)
				first = !first;
			else
				scoreString += ",";
			scoreString += "["+std::to_string(score.Team)+",'" + RootEngine::GUISystem::PreventHTMLInjections(score.Name) + "',"+std::to_string(score.Kills)+","+std::to_string(score.Deaths)+"]";
		}
		scoreString += "]";
		return scoreString;
	}

	bool MatchStateSystem::IsMatchOver()
	{
		ECS::Entity* matchState = m_world->GetTagManager()->GetEntityByTag("MatchState");
		TDMRuleSet* ruleSet = m_world->GetEntityManager()->GetComponent<TDMRuleSet>( matchState );

		if(ruleSet->CurrentState == MatchState::Warmup)
			return false;

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
			ECS::Entity* killerEntity = RootForce::Network::FindEntity(g_networkEntityMap, RootForce::Network::NetworkEntityID(p_killerID, RootForce::Network::ReservedActionID::CONNECT, RootForce::Network::SEQUENCE_PLAYER_ENTITY));
			RootForce::TDMRuleSet* rules = g_world->GetEntityManager()->GetComponent<RootForce::TDMRuleSet>(matchStateEntity);
			PlayerComponent* killerPlayerComponent = g_world->GetEntityManager()->GetComponent<PlayerComponent>(killerEntity);

			rules->TeamScore[killerPlayerComponent->TeamID]++;
			killerPlayerComponent->Score++;
		}
		g_world->GetEntityManager()->GetComponent<PlayerComponent>(RootForce::Network::FindEntity(g_networkEntityMap, deadNetworkID))->Deaths ++;
	}

	

}

#endif