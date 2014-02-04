#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/Network/NetworkTypes.h>

namespace RootForce
{
	struct NetworkContext;

	struct TDMRuleSet : public ECS::Component<TDMRuleSet>
	{
		float TimeLeft;
		int ScoreLimit;
		int TeamScore[3];
		~TDMRuleSet()
		{
			int i = 0;
		}
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

		float GetTimeLeft();
		int GetTeamScore(int p_team);

		static void AwardPlayerKill(Network::UserID_t p_killerID, Network::UserID_t p_deadID); //Assign score and death after a kills has been made
	private:
		NetworkContext* m_networkContext;
		RootEngine::GameSharedContext* m_gameSharedContext;
		Logging* m_logger;
	};
}