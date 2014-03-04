#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <yaml-cpp/yaml.h>
#include <memory>

namespace RootEngine
{
	struct ParticleSystemStruct
	{
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
		int			m_template;
		float		m_rotationSpeedMin;
		float		m_rotationSpeedMax;
	};

	class ParticleImporter
	{
	public:
		
		ParticleImporter(RootEngine::GameSharedContext* p_context);
		~ParticleImporter();

		std::vector<ParticleSystemStruct*>* LoadParticleSystem(const std::string p_fileName);

	private:
		RootEngine::GameSharedContext* m_context;
	};
}
