#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/Logging/Logging.h>

namespace RootForce
{

	struct TDMRuleSet : public ECS::Component<TDMRuleSet>
	{
		double TimeLeft;
		int ScoreLimit;
	};

	class GameLogicSystem : public ECS::VoidSystem
	{
	public:
		GameLogicSystem(ECS::World* p_world);
		void Process();
		bool IsMatchOver() { return m_matchOver; }
		void SetLoggingInterface(Logging* p_logger);
		void GetTimeLeft();
	private:
		Logging* m_logger;
		bool m_matchOver;
	};
}