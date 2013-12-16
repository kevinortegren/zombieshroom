#pragma once

#include <map>
#include <Utility\ECS\Include\World.h>
#include <RootSystems/Include/Transform.h>
#include <RootEngine\Render\Include\Renderer.h>
#include <RootEngine\Include\Logging\Logging.h>
#include <RootSystems\Include\Action.h>
#include <vector>

namespace RootForce
{
	/** Component owned by a player entity that will be controlled via input. */
	struct Ability : public ECS::Component<Ability>
	{
		std::string m_name;
		std::vector<Action> m_actions;
	};

	/** This system will update the player entity depending on input received. */
	class AbilitySystem : public ECS::EntitySystem
	{
	public:
		AbilitySystem(ECS::World* p_world, Render::RendererInterface* p_renderer)
			: ECS::EntitySystem(p_world), m_renderer(p_renderer)
		{
			SetUsage<Transform>();
		}
		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();

	private:	
		ECS::ComponentMapper<Transform> m_transforms;

		Render::RendererInterface* m_renderer;
		Logging* m_logger;
	};
}