#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootSystems/Include/Transform.h>
#include <RootEngine/Render/Include/ParticleSystem.h>
#include <RootEngine/Render/Include/Material.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>

extern RootEngine::GameSharedContext g_engineContext;

namespace RootForce
{
	struct ParticleEmitter : public ECS::Component<ParticleEmitter>
	{
		std::vector<RootEngine::ParticleSystemStruct*> m_particleSystems;
		std::vector<Render::ParticleSystemInterface*> m_systems;
		std::string m_name;

		float m_alive;

		ParticleEmitter()
		{
			m_alive = 1.0f;
		}

		~ParticleEmitter()
		{
			for(auto itr = m_systems.begin(); itr != m_systems.end(); itr++)
			{
				g_engineContext.m_renderer->FreeParticleSystem((*itr));
			}
		}
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
	};
}