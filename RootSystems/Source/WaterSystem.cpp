#include <RootSystems/Include/WaterSystem.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
namespace RootForce
{
	void WaterSystem::Init()
	{
		
		m_maxX = m_maxZ = 512;
		
		//Create empty texture for compute shader
		m_texture[0] = m_context->m_renderer->CreateTexture();
		m_texture[0]->CreateEmptyTexture(m_maxX, m_maxZ, Render::TextureFormat::TextureFormat::TEXTURE_RGBA32F );
		m_texture[0]->SetAccess(GL_READ_WRITE);
		m_texture[1] = m_context->m_renderer->CreateTexture();
		m_texture[1]->CreateEmptyTexture(m_maxX, m_maxZ, Render::TextureFormat::TextureFormat::TEXTURE_RGBA32F );
		m_texture[1]->SetAccess(GL_READ_WRITE);

	

		//Create render material
		m_material				= m_context->m_resourceManager->GetMaterial("water");
		m_material->m_effect	= m_context->m_resourceManager->LoadEffect("WaterCompute");

		m_computeJob.m_effect		= m_material->m_effect;
		m_computeJob.m_groupDim		= glm::uvec3(m_maxX/16, m_maxZ/16, 1);
		m_computeJob.m_textures[0]	= m_texture[0];
		m_computeJob.m_textures[1]	= m_texture[1];
		
		m_normalEffect = m_context->m_resourceManager->LoadEffect("WaterComputeNormals");
		m_computeNormalJob.m_effect = m_normalEffect;
		m_computeNormalJob.m_groupDim		= glm::uvec3(m_maxX/16, m_maxZ/16, 1);
		m_computeNormalJob.m_textures[0]	= m_computeJob.m_textures[0];
		m_computeNormalJob.m_params[Render::Semantic::XMAX]	= &m_maxX;
		m_computeNormalJob.m_params[Render::Semantic::YMAX]	= &m_maxZ;

		float speed		= 60.0f;
		float dx		= 5.0f;
		m_timeStep		= 0.016f;
		float damping	= 0.0f;

		float d		= damping*m_timeStep+2.0f;
		float e		= (speed*speed)*(m_timeStep*m_timeStep)/(dx*dx);
		m_mk1     = (damping*m_timeStep-2.0f)/ d;
		m_mk2     = (4.0f-8.0f*e) / d;
		m_mk3     = (2.0f*e) / d;

		m_computeJob.m_params[Render::Semantic::MK1]	= &m_mk1;
		m_computeJob.m_params[Render::Semantic::MK2]	= &m_mk2;
		m_computeJob.m_params[Render::Semantic::MK3]	= &m_mk3;
		m_computeJob.m_params[Render::Semantic::XMAX]	= &m_maxX;
		m_computeJob.m_params[Render::Semantic::YMAX]	= &m_maxZ;
		
		m_dt = 0;
		
	}

	void WaterSystem::Begin()
	{

	}

	void WaterSystem::Process()
	{
		m_dt += m_world->GetDelta();
		//float Pixels[64*64*4];
		if(m_dt >= m_timeStep)
		{
			m_context->m_renderer->Compute(&m_computeJob);
			/*m_computeJob.m_textures[0]->Bind(0);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, Pixels);
			m_computeJob.m_textures[0]->Unbind(0);*/

			m_computeNormalJob.m_textures[0]	= m_computeJob.m_textures[0];
			m_context->m_renderer->Compute(&m_computeNormalJob);
			std::swap(m_computeJob.m_textures[0], m_computeJob.m_textures[1]);
			m_renderable->m_material->m_textures[Render::TextureSemantic::DIFFUSE] =  m_computeJob.m_textures[1];
			m_dt = 0;
		}
	}

	void WaterSystem::End()
	{

	}

	void WaterSystem::CreateRenderable()
	{
		ECS::Entity*			waterEnt	= m_world->GetEntityManager()->CreateEntity();
		m_renderable			= m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(waterEnt);
		RootForce::Transform*	trans		= m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(waterEnt);
		trans->m_position = glm::vec3(0,0,0);
		trans->m_scale = glm::vec3(10,1,10);
		m_renderable->m_model = m_context->m_resourceManager->LoadCollada("256planeUV");
		m_renderable->m_material	= m_context->m_resourceManager->GetMaterial("waterrender");
		m_renderable->m_material->m_textures[Render::TextureSemantic::DIFFUSE] =  m_computeJob.m_textures[1];
		m_renderable->m_material->m_textures[Render::TextureSemantic::SPECULAR] = m_context->m_resourceManager->LoadTexture("water", Render::TextureType::TEXTURE_2D);
		m_renderable->m_material->m_textures[Render::TextureSemantic::NORMAL] =  m_context->m_resourceManager->LoadTexture("foam", Render::TextureType::TEXTURE_2D);
		m_renderable->m_params[Render::Semantic::EYEWORLDPOS] = &m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_world->GetTagManager()->GetEntityByTag("Camera"))->m_position;
		m_renderable->m_material->m_effect = m_context->m_resourceManager->LoadEffect("MeshWater");
		m_renderable->m_model->m_meshes[0]->SetPrimitiveType(GL_PATCHES);
		m_renderable->m_model->m_meshes[0]->SetWireFrame(false);
		glPatchParameteri(GL_PATCH_VERTICES, 3);
	}

	void WaterSystem::Disturb( int p_x, int p_z, float p_power )
	{
		//Return if out of bounds
		if(p_x >= m_maxX || p_x <= 0 || p_z >= m_maxZ || p_z <= 0)
			return;

		//Disturb 5 pixels. The one in the middle is disturbed at full power and the other 4 are disturbed at half the power
		m_computeJob.m_textures[1]->Bind(0);
		std::vector<glm::vec4> emptyData(1, glm::vec4(0,0,0,p_power));
		std::vector<glm::vec4> emptyDataHalf(1, glm::vec4(0,0,0,p_power/2.0f));
		
		glTexSubImage2D(GL_TEXTURE_2D, 0, p_x,		p_z,	1, 1, GL_RGBA, GL_FLOAT, &emptyData[0]); 
		glTexSubImage2D(GL_TEXTURE_2D, 0, p_x+1,	p_z,	1, 1, GL_RGBA, GL_FLOAT, &emptyDataHalf[0]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, p_x-1,	p_z,	1, 1, GL_RGBA, GL_FLOAT, &emptyDataHalf[0]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, p_x,		p_z+1,	1, 1, GL_RGBA, GL_FLOAT, &emptyDataHalf[0]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, p_x,		p_z+1,	1, 1, GL_RGBA, GL_FLOAT, &emptyDataHalf[0]);

		m_computeJob.m_textures[1]->Unbind(0);
	}
}
