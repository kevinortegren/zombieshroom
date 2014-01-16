#pragma once

#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/Component.h>
#include <Utility/ECS/Include/EntitySystem.h>
#include <RootSystems/Include/PlayerSystem.h>

namespace RootSystems
{
	class RespawnSystem : public ECS::EntitySystem
	{
	public:
		RespawnSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext)
			: ECS::EntitySystem(p_world)
			, m_engineContext(p_engineContext) { }
		void Init() { }
		void ProcessEntity(ECS::Entity* p_entity);
	private:
		RootEngine::GameSharedContext* m_engineContext;
	};
}