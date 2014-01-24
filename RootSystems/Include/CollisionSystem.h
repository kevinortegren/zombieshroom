#pragma once

#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Script.h>
#include <set>

namespace RootForce
{
	struct Collision : public ECS::Component<Collision>
	{
		int* m_handle;
		std::string m_meshHandle;
	};

	struct CollisionResponder : public ECS::Component<CollisionResponder>
	{
		std::set<void*> m_collidedEntities;
	};

	class CollisionSystem : public ECS::EntitySystem
	{
	public:
		CollisionSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext)
			: ECS::EntitySystem(p_world)
			, m_engineContext(p_engineContext)
		{
			SetUsage<Collision>();
			SetUsage<CollisionResponder>();
			SetUsage<Script>();
		}

		ECS::ComponentMapper<Script> m_scripts;
		ECS::ComponentMapper<CollisionResponder> m_responders;

		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();
	private:
		RootEngine::GameSharedContext* m_engineContext;
	};

}