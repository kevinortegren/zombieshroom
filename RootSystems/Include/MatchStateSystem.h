#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/GameSharedContext.h>

namespace RootForce
{

	struct TDMRuleSet : public ECS::Component<TDMRuleSet>
	{
		float TimeLeft;
		int ScoreLimit;
		int TeamOneScore;
		int TeamTwoScore;
	};

	class MatchStateSystem : public ECS::VoidSystem
	{
	public:
		MatchStateSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_gameSharedContext)
			: ECS::VoidSystem(p_world)
			, m_gameSharedContext(p_gameSharedContext) {}
		void Process();
		void UpdateDeltatime(float p_deltaTime);
		bool IsMatchOver();
		void SetLoggingInterface(Logging* p_logger);

		float GetTimeLeft();
		int GetTeamScore(int p_team);
	private:
		RootEngine::GameSharedContext* m_gameSharedContext;
		Logging* m_logger;
	};
}