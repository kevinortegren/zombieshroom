#include <RootSystems/Include/WaterSystem.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <iostream>
#include <fstream>

extern RootEngine::GameSharedContext g_engineContext;
namespace RootForce
{

	WaterSystem::WaterSystem( ECS::World* p_world, RootEngine::GameSharedContext* p_context ) 
		: ECS::EntitySystem(p_world), m_context(p_context), m_world(p_world), m_wireFrame(false), m_scale(1.0f), m_renderable(nullptr), m_pause(true), m_totalTime(0.0f), m_waterOptions(glm::vec4(0.0f, 0.0f, 0.01f, 0.0f)), m_playerWaterDeath(true), m_showDebugDraw(false)
	{
		SetUsage<RootForce::Transform>();
		SetUsage<RootForce::WaterCollider>();
	}

	WaterSystem::~WaterSystem()
	{
		delete m_textureData;
	}

	void WaterSystem::Init()
	{
		m_transform.Init(m_world->GetEntityManager());
		m_waterCollider.Init(m_world->GetEntityManager());
		
		m_texSize = 1024;
		m_scale = 40.0f;
		m_dt = 0;
		m_gridSize = 64;

		m_textureData = new float[m_texSize*m_texSize];
	}

	void WaterSystem::Begin()
	{
		if(m_pause)//Don't calculate if paused
			return;

		m_dt += m_world->GetDelta();
		m_totalTime += m_world->GetDelta();
		//Only simulate water every time step
		if(m_dt >= m_timeStep)
		{   	
			//Compute shader dispatch. New heights are calculated and stored in Texture0 and normals+previous height are calculated and stored in Texture1(Render texture);
			m_context->m_renderer->Compute(&m_computeJob);
			//Bind previous texture(Texture1) to render material. This will render the water 1 frame behind the simulation, but increases performance as there are no needs for memoryBarriers in the compute shader.
			m_renderable->m_material->m_textures[Render::TextureSemantic::DIFFUSE1] =  m_computeJob.m_textures[1];
			//Swap the textures for next simulation step
			std::swap(m_computeJob.m_textures[0], m_computeJob.m_textures[1]);
			//Reset timer and preserve non-exact time additions
			m_dt -= m_timeStep;
		}	
	}

	void WaterSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		RootForce::Transform*		transform = m_transform.Get(p_entity);
		RootForce::WaterCollider*	waterCollider = m_waterCollider.Get(p_entity);
		
		if(m_pause)
			return;

		float waterHeight = GetWaterHeight();

		if(m_showDebugDraw)
		{
			m_context->m_renderer->AddLine(glm::vec3(transform->m_position.x, transform->m_position.y - waterCollider->m_radius/2.0f,  transform->m_position.z), glm::vec3(transform->m_position.x,  transform->m_position.y + waterCollider->m_radius/2.0f,  transform->m_position.z) ,glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
			m_context->m_renderer->AddLine(glm::vec3(transform->m_position.x-500.0f, waterHeight,  transform->m_position.z) , glm::vec3(transform->m_position.x+500.0f, waterHeight,  transform->m_position.z) ,glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
			m_context->m_renderer->AddLine(glm::vec3(transform->m_position.x, waterHeight,  transform->m_position.z-500.0f) , glm::vec3(transform->m_position.x, waterHeight,  transform->m_position.z+500.0f) ,glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		}
		//If over water, set edge time to 0 and return
		if(transform->m_position.y > waterHeight + waterCollider->m_radius/2.0f)
		{
			waterCollider->m_waterState = RootForce::WaterState::WaterState::OVER_WATER;
			waterCollider->m_edgeWaterTime = 0.0f;
			return;
		} //if under water, set edge time to 0 and return
		else if(transform->m_position.y < waterHeight - waterCollider->m_radius/2.0f)
		{
			waterCollider->m_waterState = RootForce::WaterState::WaterState::UNDER_WATER;
			waterCollider->m_edgeWaterTime = 0.0f;
			return;
		}
		else //if by the edge of the water, start disturbing at given interval
			
		if(waterCollider->m_edgeWaterTime <= 0.0f && glm::distance(glm::vec2(waterCollider->m_prevPos.x, waterCollider->m_prevPos.z) , glm::vec2(transform->m_position.x, transform->m_position.z)) > 5.0f )
		{					
			//Disturb
			if(waterCollider->m_waterState ==  RootForce::WaterState::WaterState::OVER_WATER)
				Disturb(transform->m_position.x, transform->m_position.z, -waterCollider->m_disturbPower, waterCollider->m_radius);
			else if(waterCollider->m_waterState ==  RootForce::WaterState::WaterState::UNDER_WATER)
				Disturb(transform->m_position.x, transform->m_position.z, waterCollider->m_disturbPower, waterCollider->m_radius);
			else//if previous water state was EDGE_WATER we disturb 1/3 of the power
				Disturb(transform->m_position.x, transform->m_position.z, waterCollider->m_disturbPower/3.0f, waterCollider->m_radius);
			waterCollider->m_prevPos = transform->m_position;
			waterCollider->m_edgeWaterTime = waterCollider->m_disturbInterval;
		}
		waterCollider->m_waterState = RootForce::WaterState::WaterState::EDGE_WATER;
		//Decrease edge time
		waterCollider->m_edgeWaterTime -= m_world->GetDelta();
	}

	void WaterSystem::End()
	{

	}

	void WaterSystem::CreateWater(float p_height)
	{
		//Don't create water if there is no water on the level
		if(p_height == -99999.0f)
			return;

		g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Pouring water into level!");

		//Create empty textures for compute shader swap. Texture 0 and 1 are used for height values and texture 2 is used for normals.
		m_texture[0] = m_context->m_resourceManager->CreateTexture("computeTex1");
		m_texture[0]->CreateEmptyTexture(m_texSize, m_texSize, Render::TextureFormat::TextureFormat::TEXTURE_R32 );
		m_texture[0]->SetAccess(GL_READ_WRITE);
		m_texture[1] = m_context->m_resourceManager->CreateTexture("computeTex2");
		m_texture[1]->CreateEmptyTexture(m_texSize, m_texSize, Render::TextureFormat::TextureFormat::TEXTURE_R32 );
		m_texture[1]->SetAccess(GL_READ_WRITE);
		m_texture[2] = m_context->m_resourceManager->CreateTexture("computeTex3");
		m_texture[2]->CreateEmptyTexture(m_texSize, m_texSize, Render::TextureFormat::TextureFormat::TEXTURE_RG16 );
		m_texture[2]->SetAccess(GL_READ_WRITE);

		//std::vector<glm::u8vec4> emptyData(m_texSize * m_texSize, glm::u8vec4(127,255,127,0));
		
		//m_texture[2]->BufferData(&emptyData[0]);

		//Create compute effect
		m_effect = m_context->m_resourceManager->LoadEffect("WaterCompute");

		//Setup compute job
		m_computeJob.m_effect		= m_effect;
		m_computeJob.m_groupDim		= glm::uvec3(m_texSize/16, m_texSize/16, 1);
		m_computeJob.m_textures[0]	= m_texture[0];
		m_computeJob.m_textures[1]	= m_texture[1];
		m_computeJob.m_textures[2]  = m_texture[2];
		m_computeJob.m_params[Render::Semantic::MK1]	= &m_mk1;
		m_computeJob.m_params[Render::Semantic::MK2]	= &m_mk2;
		m_computeJob.m_params[Render::Semantic::MK3]	= &m_mk3;
		m_computeJob.m_params[Render::Semantic::DX]		= &m_dx;
		m_computeJob.m_params[Render::Semantic::XMAX]	= &m_texSize;

		//Set standard values
		m_speed		= 13.0f;
		m_dx		= (m_scale*64.0f) / m_texSize;
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
		CreateWaterMesh();
		m_renderable->m_material	= m_context->m_renderer->CreateMaterial("waterrender");
		
		//Set textures to renderable
		m_renderable->m_material->m_textures[Render::TextureSemantic::GLOW]		= m_context->m_resourceManager->LoadTexture("SkyBox", Render::TextureType::TEXTURE_CUBEMAP); 
		m_renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_G] = m_context->m_resourceManager->LoadTexture("foam", Render::TextureType::TEXTURE_2D);
		m_renderable->m_material->m_textures[Render::TextureSemantic::NORMAL]	= m_computeJob.m_textures[2];
		m_renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_B] = m_context->m_resourceManager->LoadTexture("waternormal", Render::TextureType::TEXTURE_2D);
		m_renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_B]->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
		m_renderable->m_material->m_textures[Render::TextureSemantic::TEXTURE_B]->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
		//Set pass
		m_renderable->m_pass = RenderPass::RENDERPASS_WATER;
		m_renderable->m_forward = true;
		m_renderable->m_refractive = true;

		//Total running time
		m_renderable->m_params[Render::Semantic::LIFETIMEMIN] = &m_totalTime;

		//DX
		m_renderable->m_params[Render::Semantic::LIFETIMEMAX] = &m_dx;

		//Water options
		m_renderable->m_params[Render::Semantic::COLOR] = &m_waterOptions;

		//Directional light
		m_renderable->m_params[Render::Semantic::COLOREND] = &m_context->m_renderer->GetDirectionalLight()->m_color;
		m_renderable->m_params[Render::Semantic::DIRECTION] = &m_context->m_renderer->GetDirectionalLight()->m_direction;

		//Camera position in world space
		m_renderable->m_params[Render::Semantic::EYEWORLDPOS]					= &m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_world->GetTagManager()->GetEntityByTag("Camera"))->m_position; 
		m_renderable->m_material->m_effect										= m_context->m_resourceManager->LoadEffect("MeshWater");

		m_world->GetTagManager()->RegisterEntity("Water", waterEnt);
		m_world->GetTagManager()->RegisterEntity("NonExport", waterEnt);

		g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::SUCCESS, "Water created!");

		//Start with some init disturbs
		LoadWater();

#ifndef RENDER_USE_COMPUTE
		ResetWater();
#endif
		//Run water simulation
		m_pause = false;
	}

	void WaterSystem::CreateWaterMesh()
	{
		if(m_context->m_resourceManager->GetModel("WaterModel"))
		{
			m_renderable->m_model = m_context->m_resourceManager->GetModel("WaterModel");
			return;
		}

		std::vector<Render::Vertex1P1N1UV1T1BT> vertices;
		std::vector<unsigned int> indices;

		for (float x = 0.0f; x < (float)m_gridSize; ++x)
		{
			for (float z = 0.0f; z < (float)m_gridSize; ++z)
			{
				Render::Vertex1P1N1UV1T1BT v;
				v.m_pos		= glm::vec3(x-(float)m_gridSize/2.0f, 0.0f, z-(float)m_gridSize/2.0f);
				v.m_normal	= glm::vec3(0.0f, 1.0f, 0.0f);
				v.m_UV		= glm::vec2(x/(float)m_gridSize, z/(float)m_gridSize);
				v.m_tangent = glm::vec3(0.0f);//TODO
				v.m_bitangent = glm::vec3(0.0f);//TODO
				vertices.push_back(v);
			}
		}

		for (unsigned int x = 0; x < (unsigned int)(m_gridSize-1); ++x)
		{
			for (unsigned int z = 0; z < (unsigned int)(m_gridSize-1); ++z)
			{
				unsigned int  start = x * m_gridSize + z;
				indices.push_back(start);
				indices.push_back(start + 1);
				indices.push_back(start + m_gridSize + 1);
				indices.push_back(start + m_gridSize + 1);
				indices.push_back(start + m_gridSize);
				indices.push_back(start);
			}
		}
		

		m_renderable->m_model = m_context->m_resourceManager->CreateModel("WaterModel");
		m_renderable->m_model->m_meshes[0]->SetVertexBuffer(m_context->m_renderer->CreateBuffer(GL_ARRAY_BUFFER));
		m_renderable->m_model->m_meshes[0]->SetElementBuffer(m_context->m_renderer->CreateBuffer(GL_ELEMENT_ARRAY_BUFFER));
		m_renderable->m_model->m_meshes[0]->SetVertexAttribute(m_context->m_renderer->CreateVertexAttributes());
		m_renderable->m_model->m_meshes[0]->CreateVertexBuffer1P1N1UV1T1BT(&vertices[0], vertices.size());
		m_renderable->m_model->m_meshes[0]->CreateIndexBuffer(&indices[0], indices.size());

		//Set primitive type to GL_PATCHES because we use tesselation
		m_renderable->m_model->m_meshes[0]->SetPrimitiveType(GL_PATCHES); 
		//m_renderable->m_model->m_meshes[0]->SetNoCulling(true); 
	}

	void WaterSystem::Disturb( float p_x, float p_z, float p_power, int p_radius )
	{
#ifdef RENDER_USE_COMPUTE
		glm::vec2 waterPos = WorldSpaceToWaterSpace(glm::vec2(p_x, p_z));
		//g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Disturb position: x: %f, z: %f", waterPos.x, waterPos.y);

		//Return if out of bounds
		if(waterPos.x >= m_texSize || waterPos.x <= 0 || waterPos.y >= m_texSize || waterPos.y <= 0)
			return;

		//Calculate disturb texels with a radius-to-pixelcircle algorithm
		m_computeJob.m_textures[1]->Bind(0);
		std::vector<glm::vec3> plumsPos;
		for(int i = p_radius; abs(i) <= p_radius; i--)
		{
			plumsPos.push_back(glm::vec3(waterPos.x, waterPos.y + (float)i, (( abs(i)/(float)p_radius)-0.5f)*p_power));

			for (int j = 1; sqrt(pow((float)j,2) + pow((float)i,2)) <= (float)p_radius; j++)
			{
				plumsPos.push_back(glm::vec3(waterPos.x + (float)j, waterPos.y + (float)i,((((sqrt(pow((float)j,2) + pow((float)i,2)))/(float)p_radius)-0.5f)*p_power)));
			}

			for (int j = -1; sqrt(pow((float)j,2) + pow((float)i,2)) <= (float)p_radius; j--)
			{
				plumsPos.push_back(glm::vec3(waterPos.x + (float)j, waterPos.y + (float)i,  ((((sqrt(pow((float)j,2) + pow((float)i,2)))/(float)p_radius)-0.5f)*p_power)));
			}
		}

		//m_context->m_profiler->BeginGPUTimer();
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, m_textureData);
		//double time = m_context->m_profiler->EndGPUTimer();
		//m_context->m_logger->LogText(LogTag::RENDER, LogLevel::DEBUG_PRINT, "Compute time: %f ms", time);
		
		for(unsigned i = 0; i < plumsPos.size(); ++i)
		{
			//Check out of range
			if((int)plumsPos[i].x <= 0 || (int)plumsPos[i].y <= 0 || (int)plumsPos[i].x >= m_texSize || (int)plumsPos[i].y >= m_texSize)
				continue;
			plumsPos[i].z += m_textureData[(int)plumsPos[i].x + m_texSize * (int)plumsPos[i].y];
			glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)plumsPos[i].x, (GLint)plumsPos[i].y, 1, 1, GL_RED, GL_FLOAT, &plumsPos[i].z);//0.005ms
		}

		m_computeJob.m_textures[1]->Unbind(0);

		//g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Disturb samples %d", plumsPos.size());
#endif
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

	void WaterSystem::ToggleReflections()
	{
		g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Water reflections toggled!");
		m_waterOptions.x = (m_waterOptions.x == 0.0f) ? 1.0f : 0.0f;
	}

	void WaterSystem::ToggleRefractions()
	{
		g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Water refractions toggled!");
		m_waterOptions.w = (m_waterOptions.w == 0.0f) ? 1.0f : 0.0f;
	}

	void WaterSystem::ToggleNormalMaps()
	{
		g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Water normalmaps toggled!");
		m_waterOptions.y = (m_waterOptions.y == 0.0f) ? 1.0f : 0.0f;
	}

	void WaterSystem::ToggleCollideDeath()
	{
		g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Water collision death toggled!");
		m_playerWaterDeath = m_playerWaterDeath ? false : true;
	}

	void WaterSystem::ToggleCollisionDebugDraw()
	{
		g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Water collision draw toggled!");
		m_showDebugDraw = m_showDebugDraw ? false : true;
	}

	void WaterSystem::IncreaseSpeed()
	{
		m_speed += 2.0f;
		if(m_speed > 500.0f)
			m_speed = 500.0f;
		SetSpeed(m_speed);
	}

	void WaterSystem::DecreaseSpeed()
	{
		m_speed -= 2.0f;
		if(m_speed < 0.0f)
			m_speed = 0.0f;
		SetSpeed(m_speed);
	}

	void WaterSystem::SetSpeed( float p_speed )
	{
		m_speed = p_speed;
		g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Water speed set to: %f", p_speed);
		CalculateWaterConstants();
	}

	void WaterSystem::IncreaseDamping()
	{
		m_damping += 0.1f;
		if(m_damping > 1.0f)
			m_damping = 1.0f;
		SetDamping(m_damping);
	}

	void WaterSystem::DecreaseDamping()
	{
		m_damping -= 0.1f;
		if(m_damping < 0.0f)
			m_damping = 0.0f;
		SetDamping(m_damping);
	}

	void WaterSystem::SetDamping( float p_damping )
	{
		m_damping = p_damping;
		g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Water damping set to: %f", p_damping);
		CalculateWaterConstants();
	}

	float WaterSystem::GetWaterHeight()
	{
		return m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_world->GetTagManager()->GetEntityByTag("Water"))->m_position.y;
	}

	void WaterSystem::SetWaterHeight( float p_height )
	{
		m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_world->GetTagManager()->GetEntityByTag("Water"))->m_position.y = p_height;
	}

	bool WaterSystem::ValidValues()
	{
		float calc = (m_dx/(2*m_timeStep))*std::sqrtf(m_damping*m_timeStep+2);
		if(m_speed > 0 && m_speed < calc)
			return true;
		else 
			return false;
	}

	void WaterSystem::ResetWater()
	{
		float* nada = new float[m_texSize*m_texSize]();
		m_computeJob.m_textures[1]->Bind(0);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_texSize, m_texSize, GL_RED, GL_FLOAT, nada);
		m_computeJob.m_textures[1]->Unbind(0);
		m_computeJob.m_textures[0]->Bind(0);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_texSize, m_texSize, GL_RED, GL_FLOAT, nada);
		m_computeJob.m_textures[0]->Unbind(0);
		delete nada;
		g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Water has been reset!");
	}

	glm::vec2 WaterSystem::WorldSpaceToWaterSpace( glm::vec2 p_worldSpace )
	{
		float scaleHalfWidth = ((float)m_gridSize/2.0f) * m_scale;
		return glm::vec2((p_worldSpace.x + scaleHalfWidth) * m_texSize, (p_worldSpace.y + scaleHalfWidth) * m_texSize) / (scaleHalfWidth*2.0f);
	}

	void WaterSystem::ParseCommands(std::stringstream* p_data )
	{
		std::string module;
		std::string param;
		std::string value;

		std::getline(*p_data, module, ' ');
		std::getline(*p_data, module, ' ');
		
		if(module == "low")
		{	
			m_context->m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Set water settings to LOW");
			//Reflections off
			m_waterOptions.x = 1.0f;
			//Normal map off
			m_waterOptions.y = 1.0f;
			//Refractions off
			m_waterOptions.w = 1.0f;
			//Reset water
			ResetWater();
			//No Compute and water collisions
			m_pause = true;
		}
		else if(module == "medium")
		{	
			m_context->m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Set water settings to MEDIUM");
			//Reflections off
			m_waterOptions.x = 1.0f;
			//Normal map on
			m_waterOptions.y = 0.0f;
			//Refractions on
			m_waterOptions.w = 0.0f;
			//Start water
			m_pause = false;
			//Init water
			LoadWater();
		}
		else if(module == "high")
		{	
			m_context->m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Set water settings to HIGH");
			//Reflections on
			m_waterOptions.x = 0.0f;
			//Normal map on
			m_waterOptions.y = 0.0f;
			//Refractions on
			m_waterOptions.w = 0.0f;
			//Start water
			m_pause = false;
			//Init water
			LoadWater();
		}
		else if(module == "collidedraw" || module == "cd")
		{	
			ToggleCollisionDebugDraw();
		}
		else if(module == "collide" || module == "c")
		{	
			ToggleCollideDeath();
		}
		else if(module == "wireframe" || module == "wf")
		{	
			ToggleWireFrame();

		}
		else if(module == "pause" || module == "p")
		{	
			TogglePause();
		}
		else if(module == "reset" || module == "r")
		{	
			ResetWater();
		}
		else if(module == "refl")
		{	
			ToggleReflections();
		}
		else if(module == "norm")
		{	
			ToggleNormalMaps();
		}
		else if(module == "depth")
		{	
			std::getline(*p_data, param, ' ');
			float depthFactor = (float)atof(param.c_str());
			m_context->m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Set water depth factor to %f", depthFactor);
			m_waterOptions.z = depthFactor;
		}
		else if(module == "refr")
		{	
			ToggleRefractions();
		}
		else if(module == "init")
		{
			LoadWater();
		}
		else if(module == "damping" || module == "d")
		{	
			std::getline(*p_data, param, ' ');
			std::getline(*p_data, value, ' ');

			if(param == "increase" || param == "i")
			{
				IncreaseDamping();
			}
			else if(param == "decrease" || param == "d")
			{
				DecreaseDamping();
			}
			else if(param == "set" || param == "s")
			{
				SetDamping((float)atof(value.c_str()));
			}
			else if(param == "help")
			{
				m_context->m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "[WATER DAMPING COMMANDS]");
				m_context->m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/w d i - Increase water damping");
				m_context->m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/w d d - Decrease water damping");
				m_context->m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/w d s [float] - Set water damping to X(float)");
			}
		}
		else if(module == "speed" || module == "s")
		{
			std::getline(*p_data, param, ' ');
			std::getline(*p_data, value, ' ');

			if(param == "increase" || param == "i")
			{
				IncreaseSpeed();
			}
			else if(param == "decrease" || param == "d")
			{
				DecreaseSpeed();
			}
			else if(param == "set" || param == "s")
			{
				SetSpeed((float)atof(value.c_str()));
			}
			else if(param == "help")
			{
				m_context->m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "[WATER SPEED COMMANDS]");
				m_context->m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/w s i - Increase water speed");
				m_context->m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/w s d - Decrease water speed");
				m_context->m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/w s s [float] - Set water speed to X(float)");
			}
		}
		else if(module == "height" || module == "h")
		{
			std::getline(*p_data, value, ' ');

			SetWaterHeight((float)atof(value.c_str()));
		}
		else if(module == "disturb" || module == "dis")
		{
			std::string param;
			std::string value;

			std::getline(*p_data, param, ' ');
			std::getline(*p_data, value, ' ');

			ECS::Entity* player = m_world->GetTagManager()->GetEntityByTag("Player");
			RootForce::Transform* trans =  m_world->GetEntityManager()->GetComponent<RootForce::Transform>(player);
			Disturb(trans->m_position.x, trans->m_position.z, (float)atof(param.c_str()), (int)atoi(value.c_str()));
		}
		else if(module == "help")
		{
			m_context->m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "[WATER COMMANDS]");
			m_context->m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/w p - Toggle water simulation pause");
			m_context->m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/w wf - Toggle water wireframe mode");
			m_context->m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/w refl - Toggle reflections");
			m_context->m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/w refr - Toggle refractions");
			m_context->m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/w norm - Toggle normal maps");
			m_context->m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/w c - Toggle water death collide");
			m_context->m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/w cd - Toggle water collision debug draw");
			m_context->m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/w depth [float]	- Set water depth factor X(float)");
			m_context->m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/w r - Reset the water simulation");
			m_context->m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/w init - Init water movement with default values");
			m_context->m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/w dis [float] [int] - Disturb water at player position by power(float) and radius(int)");
			m_context->m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/w h [float] - Set water height to X(float)");
			m_context->m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/w d - Damping settings");
			m_context->m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/w s - Speed settings");
		}
		else if(module == "save")
		{
			SaveWater();
		}
		
	}

	void WaterSystem::SaveWater()
	{
		m_context->m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Saving water movements...");

		m_computeJob.m_textures[0]->Bind(0);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, m_textureData);
		std::ofstream fout0 (g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets/Other/water0.init", std::ios::binary);
		fout0.write ((char*)&m_textureData[0], m_texSize*m_texSize*sizeof(float));
		m_computeJob.m_textures[0]->Unbind(0);

		m_computeJob.m_textures[1]->Bind(0);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, m_textureData);
		std::ofstream fout1 (g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets/Other/water1.init", std::ios::binary);
		fout1.write ((char*)&m_textureData[0], m_texSize*m_texSize*sizeof(float));
		m_computeJob.m_textures[1]->Unbind(0);

		m_context->m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Water is saved!");
	}

	void WaterSystem::LoadWater()
	{
		m_context->m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Loading water movements...");

		std::ifstream textureFile0 (g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets/Other/water0.init", std::ios::in|std::ios::binary);
		if (textureFile0.is_open())
		{
			textureFile0.read ((char*)&m_textureData[0], m_texSize*m_texSize*sizeof(float));
			textureFile0.close();
			m_computeJob.m_textures[0]->BufferData((void*)&m_textureData[0]);
		}
		else 
			g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::NON_FATAL_ERROR, "Init texture 0 not loaded!"); 

		std::ifstream textureFile1 (g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets/Other/water1.init", std::ios::in|std::ios::binary);
		if (textureFile1.is_open())
		{
			textureFile1.read ((char*)&m_textureData[0], m_texSize*m_texSize*sizeof(float));
			textureFile1.close();
			m_computeJob.m_textures[1]->BufferData((void*)&m_textureData[0]);
		}
		else 
			g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::NON_FATAL_ERROR, "Init texture 1 not loaded!");

		m_context->m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Water is moving!");
	}

}
