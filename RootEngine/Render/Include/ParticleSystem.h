#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

#include <RootEngine\Render\Include\Buffer.h>
#include <RootEngine\Render\Include\VertexAttributes.h>
#include <RootEngine\Render\Include\Effect.h>

#define RENDER_MAXPARTCILES 1000

namespace Render
{
	struct ParticleVertex
	{
		glm::vec3 m_initialPos;
		glm::vec3 m_initialVel;
		glm::vec2 m_size;
		float m_life; // -1 is emitter particle. 
	};

	class ParticleSystem
	{
	public:
		void Init();
		void Update(float dt);

	private:
		bool m_first;
		int m_currentVB;
		int m_currentTFB;
		GLuint m_transformFeedback[2];
		Render::Buffer m_vertexBuffer[2];
		Render::VertexAttributes* m_attributes[2];
		Render::EffectInterface* m_effect;
	};
}