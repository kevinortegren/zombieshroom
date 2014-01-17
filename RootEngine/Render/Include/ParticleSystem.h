#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

#include <RootEngine\Render\Include\Mesh.h>
#include <RootEngine\Render\Include\Effect.h>

#include <array>
#include <stack>
#include <random>

#define RENDER_NUM_PARTCILES 10000
#define RENDER_NUM_PARTICLESYSTEMS 100
#define RENDER_NUM_RANDOMVECTORS 1000

namespace Render
{
	class GLRenderer;

	struct ParticleVertex
	{
		glm::vec3 m_initialPos;
		glm::vec3 m_initialVel;
		glm::vec2 m_size;
		float m_age;
		float m_type; // 0 = emitter;
	};

	struct ParticleSystemDescription
	{
		glm::vec3 m_initalPos;
		glm::vec3 m_initalVel;
		glm::vec2 m_size;
	};

	class ParticleSystemInterface
	{
	public:
		virtual void Init(GLRenderer* p_renderer, const ParticleSystemDescription& p_desc, unsigned p_slot) = 0;
		virtual void Update() = 0;
		virtual Render::MeshInterface* GetMesh() = 0;
	};

	class ParticleSystem : public ParticleSystemInterface
	{
	public:
		friend class ParticleSystemHandler;

		void Init(GLRenderer* p_renderer, const ParticleSystemDescription& p_desc, unsigned p_slot);
		void Update();
		Render::MeshInterface* GetMesh();

	private:
		unsigned m_slot;
		bool m_first;
		int m_currentVB;
		int m_currentTFB;
		std::shared_ptr<Render::MeshInterface> m_meshes[2];
	};

	class ParticleSystemHandler
	{
	public:

		ParticleSystemHandler();
		void Init();
		ParticleSystem* Create(GLRenderer* p_renderer, const ParticleSystemDescription& p_desc);
		void Free(ParticleSystem* p_system);
		void BeginTransform(float dt);
		void EndTransform();

	private:

		void InitRandomTexture();

		// Number of active particle systems.
		unsigned m_particleSystemsCount;

		std::stack<unsigned> m_emptyParticleSlots;
		std::array<ParticleSystem, RENDER_NUM_PARTICLESYSTEMS> m_particleSystems;
		
		GLuint m_randomTexture;
		std::uniform_real_distribution<float> m_floatDistrubution;
		std::default_random_engine m_generator;

		struct
		{
			float m_dt;
			float m_gameTime;

		} m_perFrameVars;

		Buffer m_perFrameBuffer;
	};
}