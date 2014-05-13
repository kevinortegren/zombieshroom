#ifndef COMPILE_LEVEL_EDITOR
#pragma once

#include <set>
#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/Network/NetworkTypes.h>
#include <RootForce/Include/HUD.h>
#include <RootSystems/Include/AbilitySpawnSystem.h>

namespace RootForce
{
	struct NetworkContext;

	enum MatchState
	{
		Warmup,
		CountDown,
		Match,
		
	};

	typedef std::pair<ECS::Entity*, ECS::Entity*> KillPairType;
	struct KillAnnouncement : public ECS::Component<KillAnnouncement>
	{
		//First is murderer and second is victim
		std::vector<KillPairType> KillPair;
		std::string AbilityName;

		KillAnnouncement()
		{
		}
	};

	struct TDMRuleSet : public ECS::Component<TDMRuleSet>
	{
		float TimeLeft;
		int ScoreLimit;
		int TeamScore[3];
		int MinPlayers;
		float CountDown;
		MatchState CurrentState;

		TDMRuleSet()
		{
			MinPlayers = 2;
			TimeLeft = 0.0f;
			ScoreLimit = 1;
			CountDown = 0;
			CurrentState = MatchState::Warmup;

			TeamScore[0] = 0;
			TeamScore[1] = 0;
			TeamScore[2] = 0;
		}
	};

	/*
		Listener interface. Can be used to listen for events from the match state system.
	*/
	class MatchStateListener
	{
	public:
		virtual void MatchStateChanged(MatchState p_newState, MatchState p_previousState) = 0;
		virtual void PlayerKilled(KillAnnouncement* p_killAnnouncement, ECS::Entity* p_murderer, ECS::Entity* p_victim) = 0;
	};

	class MatchStateSystem : public ECS::VoidSystem
	{
	public:
		MatchStateSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_gameSharedContext)
			: ECS::VoidSystem(p_world)
			, m_gameSharedContext(p_gameSharedContext) {}
		void Process();
		void UpdateDeltatime(float p_deltaTime);
		static std::string GetScoreList();
		bool IsMatchOver();
		void SetLoggingInterface(Logging* p_logger);
		void SetNetworkContext(NetworkContext* p_networkContext) { m_networkContext = p_networkContext; }
		void SetAbilitySpawnSystem(AbilitySpawnSystem* p_system) { m_abilitySpawnSystem = p_system; }

		float GetTimeLeft();
		int GetTeamScore(int p_team);

		void AddListener(MatchStateListener* p_listener);
		void RemoveListener(MatchStateListener* p_listener);
		bool IsListening(MatchStateListener* p_listener);
	private:
		NetworkContext* m_networkContext;
		RootEngine::GameSharedContext* m_gameSharedContext;
		Logging* m_logger;

		AbilitySpawnSystem* m_abilitySpawnSystem;

		std::set<MatchStateListener*> m_matchStateListeners;
	};
}

#endif
