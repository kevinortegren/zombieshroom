#include <RootSystems/Include/WaterSystem.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootEngine/Include/Logging/Logging.h>
extern RootEngine::GameSharedContext g_engineContext;
namespace RootForce
{
	void WaterSystem::Init()
	{
		m_maxX = m_maxZ = 1024;
		m_scale = 40.0f;
		m_dt = 0;
	}

	void WaterSystem::Begin()
	{

	}

	void WaterSystem::Process()
	{
		if(m_pause)//Don't calculate if paused
			return;

		m_dt += m_world->GetDelta();
		//Only simulate water every time step
		if(m_dt >= m_timeStep)
		{
			//Compute shader dispatch. New heights are calculated and stored in Texture0 and normals+previous height are calculated and stored in Texture1(Render texture);
			m_context->m_renderer->Compute(&m_computeJob);
			//Bind previous texture(Texture1) to render material. This will render the water 1 frame behind the simulation, but increases performance as there are no needs for memoryBarriers in the compute shader.
			m_renderable->m_material->m_textures[Render::TextureSemantic::DIFFUSE] =  m_computeJob.m_textures[1];
			//Swap the textures for next simulation step
			std::swap(m_computeJob.m_textures[0], m_computeJob.m_textures[1]);
			//Reset timer and preserve non-exact time additions
			m_dt -= m_timeStep;
		}
	}

	void WaterSystem::End()
	{

	}

	void WaterSystem::CreateWater(float p_height)
	{
		//Only one water entity can be created
		if(m_renderable)
		{
			g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Water entity already created!");
			return;
		}
		g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Pouring water into level!");

		//Create empty textures for compute shader swap. Texture 0 and 1 are used for height values and texture 2 is used for normals.
		m_texture[0] = m_context->m_renderer->CreateTexture();
		m_texture[0]->CreateEmptyTexture(m_maxX, m_maxZ, Render::TextureFormat::TextureFormat::TEXTURE_R32 );
		m_texture[0]->SetAccess(GL_READ_WRITE);
		m_texture[1] = m_context->m_renderer->CreateTexture();
		m_texture[1]->CreateEmptyTexture(m_maxX, m_maxZ, Render::TextureFormat::TextureFormat::TEXTURE_R32 );
		m_texture[1]->SetAccess(GL_READ_WRITE);
		m_texture[2] = m_context->m_renderer->CreateTexture();
		m_texture[2]->CreateEmptyTexture(m_maxX, m_maxZ, Render::TextureFormat::TextureFormat::TEXTURE_RGBA8 );
		m_texture[2]->SetAccess(GL_READ_WRITE);

		//Create compute effect
		m_effect = m_context->m_resourceManager->LoadEffect("WaterCompute");

		//Setup compute job
		m_computeJob.m_effect		= m_effect;
		m_computeJob.m_groupDim		= glm::uvec3(m_maxX/16, m_maxZ/16, 1);
		m_computeJob.m_textures[0]	= m_texture[0];
		m_computeJob.m_textures[1]	= m_texture[1];
		m_computeJob.m_textures[2]  = m_texture[2];
		m_computeJob.m_params[Render::Semantic::MK1]	= &m_mk1;
		m_computeJob.m_params[Render::Semantic::MK2]	= &m_mk2;
		m_computeJob.m_params[Render::Semantic::MK3]	= &m_mk3;
		m_computeJob.m_params[Render::Semantic::DX]		= &m_dx;
		m_computeJob.m_params[Render::Semantic::XMAX]	= &m_maxX;
		m_computeJob.m_params[Render::Semantic::YMAX]	= &m_maxZ;

		//Set standard values
		m_speed		= 10.0f;
		m_dx		= (m_scale*64.0f) / m_maxZ;
		m_timeStep	= 0.032f;
		m_damping	= 0.0f;

		//Calculate MK1, MK2 and MK3 with standard values
		CalculateWaterConstants();

		//Create water entity
		ECS::Entity*			waterEnt	= m_world->GetEntityManager()->CreateEntity();
		RootForce::Transform*	trans		= m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(waterEnt);
		trans->m_position = glm::vec3(0,p_height,0); //Set Y-position to in-parameter
		trans->m_scale = glm::vec3(m_scale,1,m_scale);

		//Create a renderable component for the water
		m_renderable				= m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(waterEnt);
		m_renderable->m_model		= m_context->m_resourceManager->LoadCollada("64x64grid"); //Load a grid mesh, this could be done in code instead
		m_renderable->m_material	= m_context->m_renderer->CreateMaterial("waterrender");

		//Set textures to renderable
		m_renderable->m_material->m_textures[Render::TextureSemantic::GLOW]		= m_context->m_resourceManager->LoadTexture("SkyBox", Render::TextureType::TEXTURE_CUBEMAP); 
		m_renderable->m_material->m_textures[Render::TextureSemantic::SPECULAR] = m_context->m_resourceManager->LoadTexture("water", Render::TextureType::TEXTURE_2D);
		m_renderable->m_material->m_textures[Render::TextureSemantic::NORMAL]	= m_computeJob.m_textures[2];

		//Camera position in world space
		m_renderable->m_params[Render::Semantic::EYEWORLDPOS]					= &m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_world->GetTagManager()->GetEntityByTag("Camera"))->m_position; 
		m_renderable->m_material->m_effect										= m_context->m_resourceManager->LoadEffect("MeshWater");

		//Set primitive type to GL_PATCHES because we use tesselation
		m_renderable->m_model->m_meshes[0]->SetPrimitiveType(GL_PATCHES); 
		m_world->GetTagManager()->RegisterEntity("Water", waterEnt);

		g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::SUCCESS, "Water created!");

		//Start with some init disturbs
		InitDisturb();

		//Run water simulation
		m_pause = false;
	}

	void WaterSystem::Disturb( float p_x, float p_z, float p_power )
	{
		
		float scaleHalfWidth = 32.0f * m_scale;
		glm::vec2 waterPos = glm::vec2((p_x + scaleHalfWidth) * m_maxX, (p_z + scaleHalfWidth) * m_maxZ) / (scaleHalfWidth*2.0f);
		g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Disturb position: x: %f, z: %f", waterPos.x, waterPos.y);

		//Return if out of bounds
		if(waterPos.x >= m_maxX || waterPos.x <= 0 || waterPos.y >= m_maxZ || waterPos.y <= 0)
			return;

		//Disturb 5 pixels. The one in the middle is disturbed at full power and the other 4 are disturbed at half the power
		m_computeJob.m_textures[1]->Bind(0);
		std::vector<float> emptyDataHalf(1, p_power/6.0f);
		
		//glTexSubImage2D(GL_TEXTURE_2D, 0, (int)waterPos.x,		(int)waterPos.y,	1, 1, GL_RED, GL_FLOAT, &emptyData[0]); 
		glTexSubImage2D(GL_TEXTURE_2D, 0, (int)waterPos.x+1,	(int)waterPos.y,	1, 1, GL_RED, GL_FLOAT, &emptyDataHalf[0]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, (int)waterPos.x-1,	(int)waterPos.y,	1, 1, GL_RED, GL_FLOAT, &emptyDataHalf[0]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, (int)waterPos.x,		(int)waterPos.y-1,	1, 1, GL_RED, GL_FLOAT, &emptyDataHalf[0]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, (int)waterPos.x,		(int)waterPos.y+1,	1, 1, GL_RED, GL_FLOAT, &emptyDataHalf[0]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, (int)waterPos.x+1,	(int)waterPos.y+1,	1, 1, GL_RED, GL_FLOAT, &emptyDataHalf[0]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, (int)waterPos.x-1,	(int)waterPos.y-1,	1, 1, GL_RED, GL_FLOAT, &emptyDataHalf[0]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, (int)waterPos.x+1,	(int)waterPos.y-1,	1, 1, GL_RED, GL_FLOAT, &emptyDataHalf[0]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, (int)waterPos.x-1,	(int)waterPos.y+1,	1, 1, GL_RED, GL_FLOAT, &emptyDataHalf[0]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, (int)waterPos.x+2,	(int)waterPos.y,	1, 1, GL_RED, GL_FLOAT, &emptyDataHalf[0]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, (int)waterPos.x-2,	(int)waterPos.y,	1, 1, GL_RED, GL_FLOAT, &emptyDataHalf[0]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, (int)waterPos.x,		(int)waterPos.y-2,	1, 1, GL_RED, GL_FLOAT, &emptyDataHalf[0]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, (int)waterPos.x,		(int)waterPos.y+2,	1, 1, GL_RED, GL_FLOAT, &emptyDataHalf[0]);


		m_computeJob.m_textures[1]->Unbind(0);
	}

	void WaterSystem::ToggleWireFrame()
	{
		g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Water wireframe toggled!");
		m_wireFrame = m_wireFrame ? false : true;
		m_renderable->m_model->m_meshes[0]->SetWireFrame(m_wireFrame);
	}

	void WaterSystem::TogglePause()
	{
		g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Water pause toggled!");
		m_pause = m_pause ? false : true;
	}

	void WaterSystem::CalculateWaterConstants()
	{
		if(!ValidValues())
		{
			g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Values are not valid!");
			return;
		}

		float dts2		= m_damping*m_timeStep+2.0f;
		float stsdx		= (m_speed*m_speed)*(m_timeStep*m_timeStep)/(m_dx*m_dx);

		m_mk1     = (m_damping*m_timeStep-2.0f)/ dts2;
		m_mk2     = (4.0f-8.0f*stsdx) / dts2;
		m_mk3     = (2.0f*stsdx) / dts2;
	}

	void WaterSystem::SetDamping( float p_damping )
	{
		m_damping = p_damping;
		g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Water damping set to: %f", p_damping);
		CalculateWaterConstants();
	}

	void WaterSystem::SetSpeed( float p_speed )
	{
		m_speed = p_speed;
		g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Water speed set to: %f", p_speed);
		CalculateWaterConstants();
	}

	void WaterSystem::InitDisturb()
	{
		for(int i = 0; i < 50; ++i)
		{ 
			int x = (3 + rand() % (60*(int)m_scale)) - 32*(int)m_scale;
			int z = (3 + rand() % (60*(int)m_scale)) - 32*(int)m_scale;

			Disturb((float)x, (float)z, 10.0f);
		}
	}

	void WaterSystem::IncreaseDamping()
	{
		m_damping += 0.1f;
		if(m_damping > 1.0f)
			m_damping = 1.0f;
		SetDamping(m_damping);
	}

	void WaterSystem::IncreaseSpeed()
	{
		m_speed += 2.0f;
		if(m_speed > 500.0f)
			m_speed = 500.0f;
		SetSpeed(m_speed);
	}

	void WaterSystem::DecreaseDamping()
	{
		m_damping -= 0.1f;
		if(m_damping < 0.0f)
			m_damping = 0.0f;
		SetDamping(m_damping);
	}

	void WaterSystem::DecreaseSpeed()
	{
		m_speed -= 2.0f;
		if(m_speed < 0.0f)
			m_speed = 0.0f;
		SetSpeed(m_speed);
	}

	bool WaterSystem::ValidValues()
	{
		float calc = (m_dx/(2*m_timeStep))*std::sqrtf(m_damping*m_timeStep+2);
		if(m_speed > 0 && m_speed < calc)
			return true;
		else 
			return false;
	}

	float WaterSystem::GetWaterHeight()
	{
		return m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_world->GetTagManager()->GetEntityByTag("Water"))->m_position.y;
	}

	void WaterSystem::IncreaseWaterHeight()
	{
		m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_world->GetTagManager()->GetEntityByTag("Water"))->m_position.y++;
	}

	void WaterSystem::DecreaseWaterHeight()
	{
		m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_world->GetTagManager()->GetEntityByTag("Water"))->m_position.y--;
	}

}
