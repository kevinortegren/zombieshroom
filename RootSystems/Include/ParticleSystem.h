#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootSystems/Include/Transform.h>
#include <RootEngine/Render/Include/ParticleSystem.h>
#include <RootEngine/Render/Include/Material.h>

namespace RootForce
{
	struct ParticleEmitter : public ECS::Component<ParticleEmitter>
	{
		Render::ParticleSystemInterface* m_system;
		Render::Material* m_material;
	};

	class ParticleSystem : public ECS::EntitySystem
	{
	public:
		ParticleSystem(ECS::World* p_world);

		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();
	private:
		ECS::ComponentMapper<ParticleEmitter> m_emitters;
		ECS::ComponentMapper<Transform> m_transforms;
	
		float dt;
	};
}