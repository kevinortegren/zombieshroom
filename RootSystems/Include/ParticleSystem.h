#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootSystems/Include/Transform.h>
#include <RootEngine/Render/Include/ParticleSystem.h>

namespace RootForce
{
	struct ParticleEmitter : public ECS::Component<ParticleEmitter>
	{
		Render::ParticleSystemInterface* m_system;
	};

	struct ParticleSystem : public ECS::EntitySystem
	{
		ParticleSystem(ECS::World* p_world)
			: ECS::EntitySystem(p_world)
		{
			SetUsage<ParticleEmitter>();
			SetUsage<Transform>();
		}

		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();

		ECS::ComponentMapper<ParticleEmitter> m_emitters;
		ECS::ComponentMapper<Transform> m_transforms;
		
	private:
		Logging::LoggingInterface* m_logger;
	};
}