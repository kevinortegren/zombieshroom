#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

#include <RootEngine\Render\Include\Buffer.h>
#include <RootEngine\Render\Include\VertexAttributes.h>
#include <RootEngine\Render\Include\Effect.h>

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
		virtual void Init(GLRenderer* p_renderer, Render::EffectInterface* p_effect) = 0;
		virtual void Update(float p_dt) = 0;
	};

	class ParticleSystem : public ParticleSystemInterface
	{
	public:
		ParticleSystem();
		~ParticleSystem();
		void Init(GLRenderer* p_renderer, Render::EffectInterface* p_effect);
		void Update(float p_dt);

	private:
		bool m_first;
		int m_currentVB;
		int m_currentTFB;
		GLuint m_transformFeedback[2];
		
		std::shared_ptr<Render::BufferInterface> m_vertexBuffer[2];
		std::shared_ptr<Render::VertexAttributesInterface> m_attributes[2];

		Render::EffectInterface* m_effect;
	};
}