#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/ScriptSystem.h>
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
		std::set<unsigned int> m_collidedEntityId;
	};

	class CollisionSystem : public ECS::EntitySystem
	{
	public:
		CollisionSystem(ECS::World* p_world)
			: ECS::EntitySystem(p_world)
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
	};

}