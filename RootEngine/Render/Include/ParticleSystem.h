#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

#include <RootEngine\Render\Include\Mesh.h>
#include <RootEngine\Render\Include\Effect.h>

#include <array>
#include <stack>
#include <random>

#define RENDER_NUM_PARTCILES 10000
#define RENDER_NUM_PARTICLESYSTEMS 1000
#define RENDER_NUM_RANDOMVECTORS 1000
#define RENDER_PARTICLES_UNIFORM_SIZE 1024

namespace Render
{
	class GLRenderer;

	struct ParticleVertex
	{
		glm::vec3 m_initialPos;
		glm::vec3 m_initialVel;
		glm::vec2 m_size;
		float m_age;
		float m_type; // 0 = emitter
		glm::vec4 m_color;
		glm::vec3 m_accel;
		float m_rot;
	};

	class ParticleSystemInterface
	{
	public:
		virtual void Init(GLRenderer* p_renderer, unsigned p_slot) = 0;
		virtual void Update() = 0;
		virtual Render::MeshInterface* GetMesh() = 0;
	};

	class ParticleSystem : public ParticleSystemInterface
	{
	public:
		friend class ParticleSystemHandler;

		void Init(GLRenderer* p_renderer, unsigned p_slot);
		void Update();
		Render::MeshInterface* GetMesh();

	private:
		unsigned m_slot;
		bool m_first;
		int m_currentVB;
		int m_currentTFB;
		Render::MeshInterface* m_meshes[2];
	};

	class ParticleSystemHandler
	{
	public:

		ParticleSystemHandler();
		void Init(GLRenderer* p_renderer);
		ParticleSystem* Create(GLRenderer* p_renderer);
		void SetParticleUniforms(Technique* p_technique, std::map<Render::Semantic::Semantic, void*> p_params);
		void Free(ParticleSystem* p_system);
		void BeginTransform(float dt);
		void EndTransform();

	private:

		void CreateRandom1DTexture();

		unsigned m_particleSystemsCount;

		std::stack<unsigned> m_emptyParticleSlots;
		std::array<ParticleSystem, RENDER_NUM_PARTICLESYSTEMS> m_particleSystems;
		
		std::uniform_real_distribution<float> m_floatDistrubution;
		std::default_random_engine m_generator;

		struct
		{
			float m_dt;
			float m_gameTime;

		} m_perFrameVars;

		BufferInterface* m_perFrameBuffer;
		BufferInterface* m_perObjectBuffer;

		GLuint m_randomTexture;
	};
}