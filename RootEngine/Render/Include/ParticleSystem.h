#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

#include <RootEngine\Render\Include\Mesh.h>
#include <RootEngine\Render\Include\Effect.h>

#include <array>
#include <stack>
#include <random>

#define RENDER_MAXPARTCILES 1000

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

	class ParticleSystemInterface
	{
	public:
		virtual void Init(GLRenderer* p_renderer) = 0;
		virtual void Update() = 0;
		virtual Render::MeshInterface* GetMesh() = 0;
	};

	class ParticleSystem : public ParticleSystemInterface
	{
	public:
		ParticleSystem();
		~ParticleSystem();
		void Init(GLRenderer* p_renderer);
		void Update();
		Render::MeshInterface* GetMesh();

	private:
		bool m_first;
		int m_currentVB;
		int m_currentTFB;
		std::shared_ptr<Render::MeshInterface> m_meshes[2];
	};

	//TODO: Add descriptor for particles updates.

	class ParticleSystemHandler
	{
	public:
		ParticleSystemHandler();
		void Init();
		ParticleSystem* Create(GLRenderer* p_renderer);
		void Free();
		void BeginTransform(float dt);
		void EndTransform();

	private:

		void InitRandomTexture();

		unsigned m_particleSystemsCount;
		std::stack<unsigned> m_emptyParticleSlots;
		std::array<ParticleSystem, 100> m_particleSystems;
		
		TechniqueInterface* updateTechnique;
		GLuint m_randomTexture;
		float m_gameTime;

		std::uniform_real_distribution<float> m_floatDistrubution;
		std::default_random_engine m_generator;
	};
}