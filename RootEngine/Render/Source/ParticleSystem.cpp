#include <RootEngine/Render/Include/ParticleSystem.h>
#include <RootEngine/Render/Include/Renderer.h>

#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootEngine/Render/Include/RenderExtern.h>
#include <RootEngine/Render/Include/Semantics.h>
#include <RootEngine/Include/Logging/Logging.h>

namespace Render
{
	void ParticleSystem::Init(GLRenderer* p_renderer, unsigned p_slot)
	{
		m_slot = p_slot;

		ParticleVertex particles[RENDER_NUM_PARTCILES];
		memset(particles, 0, RENDER_NUM_PARTCILES * sizeof(ParticleVertex));

		// Emitter particle.
		particles[0].m_initialPos	= glm::vec3(0.0f);
		particles[0].m_initialVel	= glm::vec3(0.0f);
		particles[0].m_size			= glm::vec2(0.0f);
		particles[0].m_age			= 0.0f;
		particles[0].m_type			= 0.0f;
		particles[0].m_color		= glm::vec4(0.0f);
		particles[0].m_accel		= glm::vec3(0.0f);
		particles[0].m_rot			= 0.0f;
		particles[0].m_rotSpeed		= 0.0f;
		
		m_meshes[0] = p_renderer->CreateMesh();
		m_meshes[0]->SetPrimitiveType(GL_POINTS);
		m_meshes[0]->SetTransformFeedback();
		m_meshes[1] = p_renderer->CreateMesh();
		m_meshes[1]->SetPrimitiveType(GL_POINTS);
		m_meshes[1]->SetTransformFeedback();

		BufferInterface* vertexBuffer[2];
		VertexAttributesInterface* attributes[2];

		vertexBuffer[0] = p_renderer->CreateBuffer(GL_ARRAY_BUFFER);
		vertexBuffer[1] = p_renderer->CreateBuffer(GL_ARRAY_BUFFER);

		for (unsigned i = 0; i < 2 ; i++) {

			GLCheck(glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_meshes[i]->GetTransformFeedback()));
			
			vertexBuffer[i]->BufferData(RENDER_NUM_PARTCILES, sizeof(ParticleVertex), particles, GL_STATIC_DRAW);
			GLCheck(glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vertexBuffer[i]->GetBufferId()));
			GLCheck(glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0)); //AMD test

			attributes[i] = p_renderer->CreateVertexAttributes();
			attributes[i]->Init(9);
			attributes[i]->SetVertexAttribPointer(vertexBuffer[i]->GetBufferId(), 0, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), 0);
			attributes[i]->SetVertexAttribPointer(vertexBuffer[i]->GetBufferId(), 1, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (char*)0 + 3 * sizeof(float));
			attributes[i]->SetVertexAttribPointer(vertexBuffer[i]->GetBufferId(), 2, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (char*)0 + 6 * sizeof(float));
			attributes[i]->SetVertexAttribPointer(vertexBuffer[i]->GetBufferId(), 3, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (char*)0 + 8 * sizeof(float));
			attributes[i]->SetVertexAttribPointer(vertexBuffer[i]->GetBufferId(), 4, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (char*)0 + 9 * sizeof(float));
			attributes[i]->SetVertexAttribPointer(vertexBuffer[i]->GetBufferId(), 5, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (char*)0 + 10 * sizeof(float));
			attributes[i]->SetVertexAttribPointer(vertexBuffer[i]->GetBufferId(), 6, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (char*)0 + 14 * sizeof(float));
			attributes[i]->SetVertexAttribPointer(vertexBuffer[i]->GetBufferId(), 7, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (char*)0 + 17 * sizeof(float));
			attributes[i]->SetVertexAttribPointer(vertexBuffer[i]->GetBufferId(), 8, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (char*)0 + 18 * sizeof(float));
		
			m_meshes[i]->SetVertexBuffer(vertexBuffer[i]);
			m_meshes[i]->SetVertexAttribute(attributes[i]);
		} 

		m_currentVB = 0;
		m_currentTFB = 1;
		m_first = true;

		m_uniforms = p_renderer->CreateBuffer(GL_UNIFORM_BUFFER);
		char data[RENDER_PARTICLES_UNIFORM_SIZE];
		memset(&data, 0, RENDER_PARTICLES_UNIFORM_SIZE);

		m_uniforms->BufferData(1, RENDER_PARTICLES_UNIFORM_SIZE, &data, GL_DYNAMIC_DRAW);
	}

	void ParticleSystem::Update()
	{
		GLCheck(glEnable(GL_RASTERIZER_DISCARD));//AMD test

		GLCheck(glBindBufferBase(GL_UNIFORM_BUFFER, RENDER_SLOT_PEROBJECT, m_uniforms->GetBufferId()));

		GLCheck(glBindBuffer(GL_ARRAY_BUFFER, m_meshes[m_currentVB]->GetVertexBuffer()->GetBufferId()));
		GLCheck(glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_meshes[m_currentTFB]->GetTransformFeedback())); 

		m_meshes[m_currentVB]->Bind();

		GLCheck(glBeginTransformFeedback(GL_POINTS));
		

		if (m_first) {
			glDrawArrays(GL_POINTS, 0, 1);
			m_first = false;
		}
		else {
			GLCheck(glDrawTransformFeedback(GL_POINTS, m_meshes[m_currentVB]->GetTransformFeedback()));
		}

		GLCheck(glEndTransformFeedback());
		GLCheck(glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0)); 

		//AMD fix
		//glFlush();//AMD test

		m_meshes[m_currentVB]->Unbind();

		std::swap(m_currentVB, m_currentTFB);
		GLCheck(glDisable(GL_RASTERIZER_DISCARD));//AMD test
	}

	Render::MeshInterface* ParticleSystem::GetMesh()
	{
		return m_meshes[m_currentVB];
	}

	Render::BufferInterface* ParticleSystem::GetUniformBuffer()
	{
		return m_uniforms;
	}

	ParticleSystem::ParticleSystem()
	{

	}

	ParticleSystem::~ParticleSystem()
	{

	}

	ParticleSystemHandler::ParticleSystemHandler()
		: m_particleSystemsCount(0)
	{
		m_perFrameVars.m_gameTime = 0;
	}

	void ParticleSystemHandler::Init(GLRenderer* p_renderer)
	{
		m_floatDistrubution = std::uniform_real_distribution<float>(0.0f, 1.0f);

		CreateRandom1DTexture();

		glActiveTexture(GL_TEXTURE0 + Render::GLRenderer::s_textureSlots[Render::TextureSemantic::RANDOM]);
		glBindTexture(GL_TEXTURE_1D, m_randomTexture);

		m_perFrameBuffer = p_renderer->CreateBuffer(GL_UNIFORM_BUFFER);
		m_perFrameBuffer->BufferData(1, sizeof(m_perFrameVars), &m_perFrameVars, GL_DYNAMIC_DRAW);

		m_perObjectBuffer = p_renderer->CreateBuffer(GL_UNIFORM_BUFFER);
		char data[RENDER_PARTICLES_UNIFORM_SIZE];
		memset(&data, 0, RENDER_PARTICLES_UNIFORM_SIZE);

		m_perObjectBuffer->BufferData(1, RENDER_PARTICLES_UNIFORM_SIZE, &data, GL_DYNAMIC_DRAW);

		for(int i = 0; i < RENDER_NUM_PARTICLESYSTEMS; ++i)
		{
			m_particleSystems[i].Init(p_renderer, i);
		}
	}

	ParticleSystem* ParticleSystemHandler::Create(GLRenderer* p_renderer)
	{
		int recycled = m_emptyParticleSlots.size();

		unsigned slot = m_particleSystemsCount;

		if(m_emptyParticleSlots.size() > 0) // Recycling of particle system slots.
		{
			slot = m_emptyParticleSlots.top();
			m_emptyParticleSlots.pop();
			m_particleSystemsCount--;
		}

		m_particleSystems[slot].m_slot = slot;

		m_particleSystemsCount++;
		
		g_context.m_logger->LogText(LogTag::PARTICLE, LogLevel::DEBUG_PRINT, "Creating particle system %d count %d recylced count %d", slot, m_particleSystemsCount, recycled);

		return &m_particleSystems[slot++];
	}

	void ParticleSystemHandler::SetParticleUniforms(ParticleSystemInterface* p_system, Technique* p_technique, std::map<Render::Semantic::Semantic, void*> p_params)
	{
		for(auto itr = p_params.begin(); itr != p_params.end(); ++itr)
		{
			int offset = p_technique->m_uniformsParams[(*itr).first];
			unsigned size = Render::GLRenderer::s_sizes[(*itr).first];

			p_system->GetUniformBuffer()->BufferSubData(offset, size, (*itr).second);
		}
	}

	void ParticleSystemHandler::Free(ParticleSystem* p_system)
	{
		ParticleVertex particles[RENDER_NUM_PARTCILES];
		memset(particles, 0, RENDER_NUM_PARTCILES * sizeof(ParticleVertex));

		p_system->m_meshes[0]->GetVertexBuffer()->BufferSubData(0, RENDER_NUM_PARTCILES * sizeof(ParticleVertex), particles);
		p_system->m_meshes[1]->GetVertexBuffer()->BufferSubData(0, RENDER_NUM_PARTCILES * sizeof(ParticleVertex), particles);
		p_system->m_currentVB = 0;
		p_system->m_currentTFB = 1;
		p_system->m_first = true;

		g_context.m_logger->LogText(LogTag::PARTICLE, LogLevel::DEBUG_PRINT, "Free %d", p_system->m_slot);
		m_emptyParticleSlots.push(p_system->m_slot);
	}

	void ParticleSystemHandler::BeginTransform(float dt)
	{
		glEnable(GL_RASTERIZER_DISCARD); 

		m_perFrameVars.m_dt = dt;
		m_perFrameVars.m_gameTime += dt;

		m_perFrameBuffer->BufferSubData(0, sizeof(m_perFrameVars), &m_perFrameVars);

		glBindBufferBase(GL_UNIFORM_BUFFER, RENDER_SLOT_PERFRAME, m_perFrameBuffer->GetBufferId());
		

		
	}

	void ParticleSystemHandler::EndTransform()
	{
		 

		 glDisable(GL_RASTERIZER_DISCARD);
	}

	void ParticleSystemHandler::CreateRandom1DTexture()
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
		glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, randomVectors.size(), 0, GL_RGB, GL_FLOAT, randomVectors.data());
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	} 
}