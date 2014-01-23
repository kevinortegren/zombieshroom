#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootSystems/Include/Transform.h>
#include <RootEngine/Render/Include/ParticleSystem.h>
#include <RootEngine/Render/Include/Material.h>

namespace RootForce
{
	struct ParticleSystemStruct
	{
		Render::ParticleSystemInterface* m_system;
		Render::Material* m_material;
		std::map<Render::Semantic::Semantic, void*> m_params;
		glm::vec3	m_position;
		float 		m_lifeTimeMin;
		float 		m_lifeTimeMax;
		float 		m_speedMin;
		float 		m_speedMax;
		glm::vec2 	m_sizeMin;
		glm::vec2 	m_sizeMax;
		glm::vec2 	m_sizeEnd;
		glm::vec4 	m_color;
		glm::vec4 	m_colorEnd;
		glm::vec3 	m_gravity;
		glm::vec3 	m_direction;
		float 		m_spread;
		float		m_spawnTime;
		std::string m_name;
		float		m_orbitSpeed;
		float		m_orbitRadius;
	};

	struct ParticleEmitter : public ECS::Component<ParticleEmitter>
	{
		std::vector<ParticleSystemStruct> m_particleSystems;
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