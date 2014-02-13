#pragma once

#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/Component.h>
#include <Utility/ECS/Include/EntitySystem.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/CollisionSystem.h>
#include <RootSystems/Include/Transform.h>

namespace RootForce
{
	struct AbilityRespawnComponent : public ECS::Component<AbilityRespawnComponent>
	{
		float Timer;
		AbilityInfo CurrentAbility;
		bool Claimed;

		AbilityRespawnComponent()
		{
			Timer = 0;
			CurrentAbility = AbilityInfo();
			Claimed = false;
		}
	};

	class AbilityRespawnSystem : public ECS::EntitySystem
	{
	public:
		AbilityRespawnSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext, std::string p_workingDir)
			: ECS::EntitySystem(p_world)
			, m_engineContext(p_engineContext), m_workingDir(p_workingDir)
		{ 
			SetUsage<RootForce::AbilityRespawnComponent>();
		}
		void Init();
		void ProcessEntity(ECS::Entity* p_entity);
		void LoadAbilities(std::string p_abilityPack);
		void AttatchComponentToPoints();
	private:
		RootEngine::GameSharedContext* m_engineContext;

		ECS::ComponentMapper<RootForce::AbilityRespawnComponent> m_respawn;
		ECS::ComponentMapper<RootForce::Transform> m_transform;
		std::vector<std::string> m_levelAbilities;
		std::string m_workingDir;
	};
}