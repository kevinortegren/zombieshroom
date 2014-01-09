#include <RootEngine/Render/Include/ParticleSystem.h>
#include <RootEngine/Render/Include/Renderer.h>

#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootEngine/Render/Include/RenderExtern.h>

namespace Render
{
	void ParticleSystem::Init(GLRenderer* p_renderer, const ParticleSystemDescription& p_desc, unsigned p_slot)
	{
		m_slot = p_slot;

		ParticleVertex particles[RENDER_NUM_PARTCILES];
		memset(particles, 0, RENDER_NUM_PARTCILES * sizeof(ParticleVertex));

		//TODO: Set values from descriptor.

		// Emitter particle.
		particles[0].m_initialPos = p_desc.m_initalPos;
		particles[0].m_initialVel = p_desc.m_initalVel;
		particles[0].m_size = p_desc.m_size;
		particles[0].m_age = 0.0f;
		particles[0].m_type = 0;

		m_meshes[0] = p_renderer->CreateMesh();
		m_meshes[0]->SetPrimitiveType(GL_POINTS);
		m_meshes[0]->SetTransformFeedback();
		m_meshes[1] = p_renderer->CreateMesh();
		m_meshes[1]->SetPrimitiveType(GL_POINTS);
		m_meshes[1]->SetTransformFeedback();

		std::shared_ptr<BufferInterface> vertexBuffer[2];
		std::shared_ptr<VertexAttributesInterface> attributes[2];

		vertexBuffer[0] = p_renderer->CreateBuffer();
		vertexBuffer[0]->Init(GL_ARRAY_BUFFER);

		vertexBuffer[1] = p_renderer->CreateBuffer();
		vertexBuffer[1]->Init(GL_ARRAY_BUFFER);

		for (unsigned i = 0; i < 2 ; i++) {

			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_meshes[i]->GetTransformFeedback());
			
			vertexBuffer[i]->BufferData(RENDER_NUM_PARTCILES, sizeof(ParticleVertex), particles);
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

	void ParticleSystem::Update()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_meshes[m_currentVB]->GetVertexBuffer()->GetBufferId());
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_meshes[m_currentTFB]->GetTransformFeedback()); 

		 m_meshes[m_currentVB]->Bind();

		 glBeginTransformFeedback(GL_POINTS);

		if (m_first) {
			glDrawArrays(GL_POINTS, 0, 1);
			m_first = false;
		}
		else {
			glDrawTransformFeedback(GL_POINTS, m_meshes[m_currentVB]->GetTransformFeedback());
		}

		 glEndTransformFeedback();

		 m_meshes[m_currentVB]->Unbind();

		 std::swap(m_currentVB, m_currentTFB);
	}

	Render::MeshInterface* ParticleSystem::GetMesh()
	{
		return m_meshes[m_currentVB].get();
	}

	ParticleSystemHandler::ParticleSystemHandler()
		: m_particleSystemsCount(0)
	{
		m_perFrameVars.m_gameTime = 0;
	}

	void ParticleSystemHandler::Init( )
	{
		m_floatDistrubution = std::uniform_real_distribution<float>(0.0f, 1.0f);

		InitRandomTexture();

		glActiveTexture(GL_TEXTURE0 + 4);
		glBindTexture(GL_TEXTURE_1D, m_randomTexture);

		m_perFrameBuffer.Init(GL_UNIFORM_BUFFER);
		m_perFrameBuffer.BufferData(1, sizeof(m_perFrameVars), &m_perFrameVars);
	}

	ParticleSystem* ParticleSystemHandler::Create(GLRenderer* p_renderer, const ParticleSystemDescription& p_desc)
	{
		unsigned slot = m_particleSystemsCount;
		if(m_emptyParticleSlots.size() > 0) // Recycling of particle system slots.
		{
			unsigned slot = m_emptyParticleSlots.top();
			m_emptyParticleSlots.pop();	
		}

		m_particleSystems[slot].Init( p_renderer, p_desc, slot );
		m_particleSystemsCount++;

		return &m_particleSystems[slot++];
	}

	void ParticleSystemHandler::Free(ParticleSystem* p_system)
	{
		m_emptyParticleSlots.push(p_system->m_slot);
	}

	void ParticleSystemHandler::BeginTransform(float dt)
	{
		glEnable(GL_RASTERIZER_DISCARD); 

		m_perFrameVars.m_dt = dt;
		m_perFrameVars.m_gameTime += dt;

		m_perFrameBuffer.BufferSubData(0, sizeof(m_perFrameVars), &m_perFrameVars);

		glBindBufferBase(GL_UNIFORM_BUFFER, 5, m_perFrameBuffer.GetBufferId());
	}

	void ParticleSystemHandler::EndTransform()
	{
		 glDisable(GL_RASTERIZER_DISCARD);
	}

	void ParticleSystemHandler::InitRandomTexture()
	{
		std::vector<glm::vec3> randomVectors;
		randomVectors.resize(RENDER_NUM_RANDOMVECTORS);

		for (unsigned int i = 0 ; i < randomVectors.size(); i++) {
			randomVectors[i].x = m_floatDistrubution(m_generator);
			randomVectors[i].y = m_floatDistrubution(m_generator);
			randomVectors[i].z = m_floatDistrubution(m_generator);
		}

		glGenTextures(1, &m_randomTexture);
		glBindTexture(GL_TEXTURE_1D, m_randomTexture);
		glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, randomVectors.size(), 0.0f, GL_RGB, GL_FLOAT, randomVectors.data());
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	} 
}