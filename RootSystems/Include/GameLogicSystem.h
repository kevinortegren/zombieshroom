#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/GameSharedContext.h>

namespace RootForce
{

	struct TDMRuleSet : public ECS::Component<TDMRuleSet>
	{
		double TimeLeft;
		int ScoreLimit;
		int TeamOneScore;
		int TeamTwoScore;
	};

	class GameLogicSystem : public ECS::VoidSystem
	{
	public:
		GameLogicSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_gameSharedContext)
			: ECS::VoidSystem(p_world)
			, m_gameSharedContext(p_gameSharedContext) {}
		void Process();
		bool IsMatchOver();
		void SetLoggingInterface(Logging* p_logger);

		double GetTimeLeft();
		int GetTeamScore(int p_team);
	private:
		RootEngine::GameSharedContext* m_gameSharedContext;
		ECS::World* m_world;
		Logging* m_logger;
	};
}