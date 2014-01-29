#include <RootSystems/Include/WaterSystem.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
namespace RootForce
{
	void WaterSystem::Init()
	{
		unsigned maxX, maxZ;
		maxX = maxZ = 256;
		
		//Create empty texture for compute shader
		m_texture[0] = m_context->m_renderer->CreateTexture();
		m_texture[0]->CreateEmptyTexture(256, 256, Render::TextureFormat::TextureFormat::TEXTURE_R32 );
		m_texture[0]->SetAccess(GL_READ_WRITE);
		m_texture[1] = m_context->m_renderer->CreateTexture();
		m_texture[1]->CreateEmptyTexture(256, 256, Render::TextureFormat::TextureFormat::TEXTURE_R32 );
		m_texture[1]->SetAccess(GL_READ_WRITE);
		
		/*m_texture[0]->Bind(0);
		std::vector<float> emptyData(1, 100);
		glTexSubImage2D(GL_TEXTURE_2D,
			0,
			10, 10,
			1,
			1,
			GL_RED,
			GL_FLOAT,
			&emptyData[0]); 
		m_texture[0]->Unbind(0);

		m_texture[1]->Bind(0);
		glTexSubImage2D(GL_TEXTURE_2D,
			0,
			10, 10,
			1,
			1,
			GL_RED,
			GL_FLOAT,
			&emptyData[0]); 
		m_texture[1]->Unbind(0);*/

		m_currTex = 0;
		/*
		//Create render material
		m_material				= m_context->m_resourceManager->GetMaterial("water");
		m_material->m_effect	= m_context->m_resourceManager->LoadEffect("WaterCompute");

		m_computeJob.m_effect		= m_material->m_effect;
		m_computeJob.m_groupDim		= glm::uvec3(256/16, 256/16, 1);
		m_computeJob.m_textures[0]	= m_texture[0];
		m_computeJob.m_textures[1]	= m_texture[1];
		
		float speed		= 1.0f;
		float dx		= 1.0f;
		float dt		= 0.008f;
		float damping	= 0;

		float d		= damping*dt+2.0f;
		float e		= (speed*speed)*(dt*dt)/(dx*dx);
		m_mk1     = (damping*dt-2.0f)/ d;
		m_mk2     = (4.0f-8.0f*e) / d;
		m_mk3     = (2.0f*e) / d;

		m_computeJob.m_params[Render::Semantic::MK1] = &m_mk1;
		m_computeJob.m_params[Render::Semantic::MK2] = &m_mk2;
		m_computeJob.m_params[Render::Semantic::MK3] = &m_mk3;
		*/
		
		
	}

	void WaterSystem::Begin()
	{

	}

	void WaterSystem::Process()
	{
		//m_context->m_renderer->Compute(&m_computeJob);
		m_texture[0]->Bind(0);
		float Pixels[256*256];
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, Pixels );
		m_texture[1]->Bind(0);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, Pixels );

		//std::swap(m_computeJob.m_textures[0], m_computeJob.m_textures[1]);
	}

	void WaterSystem::End()
	{

	}

	void WaterSystem::CreateRenderable()
	{
		ECS::Entity*			waterEnt	= m_world->GetEntityManager()->CreateEntity();
		RootForce::Renderable*	renderable	= m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(waterEnt);
		RootForce::Transform*	trans		= m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(waterEnt);
		trans->m_position = glm::vec3(0,1,0);
		renderable->m_model = m_context->m_resourceManager->LoadCollada("256plane");
		renderable->m_material	= m_context->m_resourceManager->GetMaterial("waterrender");
		renderable->m_material->m_textures[Render::TextureSemantic::DIFFUSE] =  m_texture[1];
		renderable->m_material->m_effect = m_context->m_resourceManager->LoadEffect("MeshWater");
		
	}

}
