#pragma once

#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Script.h>
#include <RootSystems/Include/CollisionInfo.h>
#include <set>
#include <iostream>

extern RootEngine::GameSharedContext g_engineContext;

namespace RootForce
{
	struct Collision : public ECS::Component<Collision>
	{
		~Collision();
		
		int* m_handle;
		std::string m_meshHandle;
	};

	struct CollisionResponder : public ECS::Component<CollisionResponder>
	{
		std::map<void*, CollisionInfo> m_collisions;
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