#include <RootEngine\Render\Include\ParticleSystem.h>
#include <RootEngine\Render\Include\Renderer.h>

namespace Render
{
	
	ParticleSystem::ParticleSystem()
	{

	}

	ParticleSystem::~ParticleSystem()
	{
		glDeleteTransformFeedbacks(2, m_transformFeedback);
	}

	void ParticleSystem::Init(GLRenderer* p_renderer)
	{
		ParticleVertex particles[RENDER_MAXPARTCILES];
		memset(particles, 0, RENDER_MAXPARTCILES*sizeof(ParticleVertex));

		// Emitter particle.
		particles[0].m_initialPos = glm::vec3(0);
		particles[0].m_initialVel = glm::vec3(1,0,0);
		particles[0].m_size = glm::vec2(1,1);
		particles[0].m_age = 0.0f;
		particles[0].m_type = 0;

		glGenTransformFeedbacks(2, m_transformFeedback);

		m_meshes[0] = p_renderer->CreateMesh();
		m_meshes[1] = p_renderer->CreateMesh();

		std::shared_ptr<BufferInterface> vertexBuffer[2];
		std::shared_ptr<VertexAttributesInterface> attributes[2];

		vertexBuffer[0] = p_renderer->CreateBuffer();
		vertexBuffer[0]->Init(GL_ARRAY_BUFFER);

		vertexBuffer[1] = p_renderer->CreateBuffer();
		vertexBuffer[1]->Init(GL_ARRAY_BUFFER);

		for (unsigned i = 0; i < 2 ; i++) {
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[i]);
			
			vertexBuffer[i]->BufferData(RENDER_MAXPARTCILES, sizeof(ParticleVertex), particles);
			glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vertexBuffer[i]->GetBufferId());

			attributes[i] = p_renderer->CreateVertexAttributes();
			attributes[i]->Init(5);
			attributes[i]->SetVertexAttribPointer(vertexBuffer[i]->GetBufferId(), 0, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), 0);
			attributes[i]->SetVertexAttribPointer(vertexBuffer[i]->GetBufferId(), 1, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (char*)0 + 3 * sizeof(float));
			attributes[i]->SetVertexAttribPointer(vertexBuffer[i]->GetBufferId(), 2, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (char*)0 + 6 * sizeof(float));
			attributes[i]->SetVertexAttribPointer(vertexBuffer[i]->GetBufferId(), 3, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (char*)0 + 8 * sizeof(float));
			attributes[i]->SetVertexAttribPointer(vertexBuffer[i]->GetBufferId(), 4, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (char*)0 + 9 * sizeof(float));

			m_meshes[i]->SetVertexBuffer(vertexBuffer[i]);
			m_meshes[i]->SetVertexAttribute(attributes[i]);
		} 

		m_currentVB = 0;
		m_currentTFB = 1;
		m_first = true;
	}

	void ParticleSystem::Update(float p_dt)
	{
		glEnable(GL_RASTERIZER_DISCARD); 

		glBindBuffer(GL_ARRAY_BUFFER, m_meshes[m_currentVB]->GetVertexBuffer()->GetBufferId());
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[m_currentTFB]); 

		 m_meshes[m_currentVB]->Bind();

		 glBeginTransformFeedback(GL_POINTS);

		if (m_first) {
			glDrawArrays(GL_POINTS, 0, 1);
			m_first = false;
		}
		else {
			glDrawTransformFeedback(GL_POINTS, m_transformFeedback[m_currentVB]);
		}

		 glEndTransformFeedback();

		 m_meshes[m_currentVB]->Unbind();

		 std::swap(m_currentVB, m_currentTFB);

		 glDisable(GL_RASTERIZER_DISCARD);
	}

	Render::MeshInterface* ParticleSystem::GetMesh()
	{
		return m_meshes[m_currentTFB].get();
	}
}