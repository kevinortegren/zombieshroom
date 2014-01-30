#include <RootSystems/Include/WaterSystem.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
namespace RootForce
{
	void WaterSystem::Init()
	{
		
		m_maxX = m_maxZ = 2048;
		
		//Create empty texture for compute shader
		m_texture[0] = m_context->m_renderer->CreateTexture();
		m_texture[0]->CreateEmptyTexture(m_maxX, m_maxZ, Render::TextureFormat::TextureFormat::TEXTURE_R32 );
		m_texture[0]->SetAccess(GL_READ_WRITE);
		m_texture[1] = m_context->m_renderer->CreateTexture();
		m_texture[1]->CreateEmptyTexture(m_maxX, m_maxZ, Render::TextureFormat::TextureFormat::TEXTURE_R32 );
		m_texture[1]->SetAccess(GL_READ_WRITE);

		
		//Create render material
		m_material				= m_context->m_resourceManager->GetMaterial("water");
		m_material->m_effect	= m_context->m_resourceManager->LoadEffect("WaterCompute");

		m_computeJob.m_effect		= m_material->m_effect;
		m_computeJob.m_groupDim		= glm::uvec3(m_maxX/16, m_maxZ/16, 1);
		m_computeJob.m_textures[0]	= m_texture[0];
		m_computeJob.m_textures[1]	= m_texture[1];
		
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
		
		if(m_dt >= m_timeStep)
		{
			m_context->m_renderer->Compute(&m_computeJob);
			std::swap(m_computeJob.m_textures[0], m_computeJob.m_textures[1]);
			m_dt = 0;
		}
	}

	void WaterSystem::End()
	{

	}

	void WaterSystem::CreateRenderable()
	{
		ECS::Entity*			waterEnt	= m_world->GetEntityManager()->CreateEntity();
		RootForce::Renderable*	renderable	= m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(waterEnt);
		RootForce::Transform*	trans		= m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(waterEnt);
		trans->m_position = glm::vec3(0,0,0);
		trans->m_scale = glm::vec3(10,1,10);
		renderable->m_model = m_context->m_resourceManager->LoadCollada("256planeUV");
		renderable->m_material	= m_context->m_resourceManager->GetMaterial("waterrender");
		renderable->m_material->m_textures[Render::TextureSemantic::DIFFUSE] =  m_texture[1];
		renderable->m_material->m_textures[Render::TextureSemantic::SPECULAR] = m_context->m_resourceManager->LoadTexture("water", Render::TextureType::TEXTURE_2D);
		renderable->m_params[Render::Semantic::EYEWORLDPOS] = &m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_world->GetTagManager()->GetEntityByTag("Camera"))->m_position;
		renderable->m_material->m_effect = m_context->m_resourceManager->LoadEffect("MeshWater");
		renderable->m_model->m_meshes[0]->SetPrimitiveType(GL_PATCHES);
		renderable->m_model->m_meshes[0]->SetWireFrame(true);
		glPatchParameteri(GL_PATCH_VERTICES, 3);
	}

	void WaterSystem::Disturb( int p_x, int p_z, float p_power )
	{
		m_texture[1]->Bind(0);
		std::vector<float> emptyData(1, p_power);
		glTexSubImage2D(GL_TEXTURE_2D,
			0,
			p_x, p_z,
			1,
			1,
			GL_RED,
			GL_FLOAT,
			&emptyData[0]); 
		m_texture[1]->Unbind(0);
	}
}
