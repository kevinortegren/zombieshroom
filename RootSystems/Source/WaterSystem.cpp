#include <RootSystems/Include/WaterSystem.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
namespace RootForce
{
	void WaterSystem::Init()
	{
		m_mesh	= m_context->m_renderer->CreateMesh();
		m_mesh->SetVertexBuffer(m_context->m_renderer->CreateBuffer(GL_ARRAY_BUFFER));	
		m_mesh->SetVertexAttribute(m_context->m_renderer->CreateVertexAttributes());

		std::vector<Render::Vertex1P1UV> vertices;
		unsigned maxX, maxZ;
		maxX = maxZ = 256;
		
		for (unsigned x = 0; x < maxX; x++)
		{
			for (unsigned z = 0; z < maxZ; z++)
			{
				Render::Vertex1P1UV v;
				v.m_pos			= glm::vec3(x,0,z);
				v.m_UV			= glm::vec2((float)x/(float)maxX, (float)z/(float)maxZ);
				vertices.push_back(v);
			}
		}
			
		m_mesh->CreateVertexBuffer1P1UV(&vertices[0], vertices.size());	

		//Create empty texture for compute shader
		m_texture[0] = m_context->m_renderer->CreateTexture();
		m_texture[0]->CreateEmptyTexture(256, 256, Render::TextureFormat::TextureFormat::TEXTURE_R32 );
		m_texture[0]->SetAccess(GL_READ_WRITE);
		m_texture[1] = m_context->m_renderer->CreateTexture();
		m_texture[1]->CreateEmptyTexture(256, 256, Render::TextureFormat::TextureFormat::TEXTURE_R32 );
		m_texture[1]->SetAccess(GL_READ_WRITE);
		
	
		m_currTex = 0;

		//Create render material
		m_material = m_context->m_resourceManager->GetMaterial("water");
		m_material->m_effect = m_context->m_resourceManager->LoadEffect("WaterCompute");

		m_computeJob.m_effect = m_material->m_effect;
		m_computeJob.m_groupDim = glm::uvec3(256/16, 256/16, 1);
		m_computeJob.m_textures[0]		= m_texture[0];
		m_computeJob.m_textures[1]	= m_texture[1];
		
		float speed = 1.0f;
		float dx = 1.0f;
		float dt = 0.008f;
		float damping = 0;

		float d = damping*dt+2.0f;
		float e = (speed*speed)*(dt*dt)/(dx*dx);
		m_mk1     = (damping*dt-2.0f)/ d;
		m_mk2     = (4.0f-8.0f*e) / d;
		m_mk3     = (2.0f*e) / d;

	/*	m_computeJob.m_params[Render::Semantic::MK1] = &m_mk1;
		m_computeJob.m_params[Render::Semantic::MK2] = &m_mk2;
		m_computeJob.m_params[Render::Semantic::MK3] = &m_mk3;*/
	}

	void WaterSystem::Begin()
	{

	}

	void WaterSystem::Process()
	{
		m_context->m_renderer->Compute(&m_computeJob);
		m_computeJob.m_textures[0]->Bind(0);
		float Pixels[256*256];
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, Pixels );
		m_computeJob.m_textures[1]->Bind(0);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, Pixels );

		std::swap(m_computeJob.m_textures[0], m_computeJob.m_textures[1]);
	}

	void WaterSystem::End()
	{

	}

}
