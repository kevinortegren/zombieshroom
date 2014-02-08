#include <RootSystems/Include/WaterSystem.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootEngine/Include/Logging/Logging.h>
extern RootEngine::GameSharedContext g_engineContext;
namespace RootForce
{

	WaterSystem::WaterSystem( ECS::World* p_world, RootEngine::GameSharedContext* p_context ) 
		: ECS::EntitySystem(p_world), m_context(p_context), m_world(p_world), m_wireFrame(false), m_scale(1.0f), m_renderable(nullptr), m_pause(true)
	{
		SetUsage<RootForce::Transform>();
		SetUsage<RootForce::WaterCollider>();
	}

	WaterSystem::~WaterSystem()
	{
	}

	void WaterSystem::Init()
	{
		m_transform.Init(m_world->GetEntityManager());
		m_waterCollider.Init(m_world->GetEntityManager());
		
		m_maxX = m_maxZ = 1024;
		m_scale = 40.0f;
		m_dt = 0;
		m_gridSize = 64;
	}

	void WaterSystem::Begin()
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

	void WaterSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		RootForce::Transform*		transform = m_transform.Get(p_entity);
		RootForce::WaterCollider*	waterCollider = m_waterCollider.Get(p_entity);

		if(m_pause)
			return;

		float waterHeight = GetWaterHeight();

		//If over water, set edge time to 0 and return
		if(transform->m_position.y > waterHeight + waterCollider->m_radius)
		{
			waterCollider->m_waterState = RootForce::WaterState::WaterState::OVER_WATER;
			waterCollider->m_edgeWaterTime = 0.0f;
			return;
		} //if under water, set edge time to 0 and return
		else if(transform->m_position.y < waterHeight - waterCollider->m_radius)
		{
			waterCollider->m_waterState = RootForce::WaterState::WaterState::UNDER_WATER;
			waterCollider->m_edgeWaterTime = 0.0f;
			return;
		}
		else //if by the edge of the water, start disturbing at given interval
			waterCollider->m_waterState = RootForce::WaterState::WaterState::EDGE_WATER;

		if(waterCollider->m_edgeWaterTime <= 0.0f && glm::distance(glm::vec2(waterCollider->m_prevPos.x, waterCollider->m_prevPos.z) , glm::vec2(transform->m_position.x, transform->m_position.z)) > 20.0f )
		{	//Disturb
			Disturb(transform->m_position.x, transform->m_position.z, waterCollider->m_disturbPower);
			waterCollider->m_prevPos = transform->m_position;
			waterCollider->m_edgeWaterTime = waterCollider->m_disturbInterval;
		}
		//Decrease edge time
		waterCollider->m_edgeWaterTime -= m_world->GetDelta();
	}

	void WaterSystem::End()
	{

	}

	void WaterSystem::CreateWater(float p_height)
	{
		//Don't create water if there is no water on the level
		if(p_height == 0)
			return;

		g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Pouring water into level!");

		//Create empty textures for compute shader swap. Texture 0 and 1 are used for height values and texture 2 is used for normals.
		m_texture[0] = m_context->m_resourceManager->CreateTexture("computeTex1");
		m_texture[0]->CreateEmptyTexture(m_maxX, m_maxZ, Render::TextureFormat::TextureFormat::TEXTURE_R32 );
		m_texture[0]->SetAccess(GL_READ_WRITE);
		m_texture[1] = m_context->m_resourceManager->CreateTexture("computeTex2");
		m_texture[1]->CreateEmptyTexture(m_maxX, m_maxZ, Render::TextureFormat::TextureFormat::TEXTURE_R32 );
		m_texture[1]->SetAccess(GL_READ_WRITE);
		m_texture[2] = m_context->m_resourceManager->CreateTexture("computeTex3");
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
		m_speed		= 11.0f;
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
		CreateWaterMesh();
		m_renderable->m_material	= m_context->m_renderer->CreateMaterial("waterrender");

		//Set textures to renderable
		m_renderable->m_material->m_textures[Render::TextureSemantic::GLOW]		= m_context->m_resourceManager->LoadTexture("SkyBox", Render::TextureType::TEXTURE_CUBEMAP); 
		m_renderable->m_material->m_textures[Render::TextureSemantic::SPECULAR] = m_context->m_resourceManager->LoadTexture("foam", Render::TextureType::TEXTURE_2D);
		m_renderable->m_material->m_textures[Render::TextureSemantic::NORMAL]	= m_computeJob.m_textures[2];

		//Camera position in world space
		m_renderable->m_params[Render::Semantic::EYEWORLDPOS]					= &m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_world->GetTagManager()->GetEntityByTag("Camera"))->m_position; 
		m_renderable->m_material->m_effect										= m_context->m_resourceManager->LoadEffect("MeshWater");

		m_world->GetTagManager()->RegisterEntity("Water", waterEnt);

		g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::SUCCESS, "Water created!");

		//Start with some init disturbs
		//InitDisturb();

		//Run water simulation
		m_pause = false;
	}

	void WaterSystem::Disturb( float p_x, float p_z, float p_power )
	{
		
		float scaleHalfWidth = ((float)m_gridSize/2.0f) * m_scale;
		glm::vec2 waterPos = glm::vec2((p_x + scaleHalfWidth) * m_maxX, (p_z + scaleHalfWidth) * m_maxZ) / (scaleHalfWidth*2.0f);
		g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Disturb position: x: %f, z: %f", waterPos.x, waterPos.y);

		//Return if out of bounds
		if(waterPos.x >= m_maxX || waterPos.x <= 0 || waterPos.y >= m_maxZ || waterPos.y <= 0)
			return;

		//Calculate disturb texels with a radius-to-pixelcircle algorithm
		m_computeJob.m_textures[1]->Bind(0);
		int brushSize = 20;
		std::vector<glm::vec3> plumsPos;
		for(int i = brushSize; abs(i) <= brushSize; i--)
		{
			if(i != 0)
			plumsPos.push_back(glm::vec3(waterPos.x, waterPos.y + (float)i, (( abs(i)/(float)brushSize)-0.5f)*p_power));

			for (int j = 1; sqrt(pow((float)j,2) + pow((float)i,2)) <= (float)brushSize; j++)
			{
				plumsPos.push_back(glm::vec3(waterPos.x + (float)j, waterPos.y + (float)i,((((sqrt(pow((float)j,2) + pow((float)i,2)))/(float)brushSize)-0.5f)*p_power)));
			}

			for (int j = -1; sqrt(pow((float)j,2) + pow((float)i,2)) <= (float)brushSize; j--)
			{
				plumsPos.push_back(glm::vec3(waterPos.x + (float)j, waterPos.y + (float)i,  ((((sqrt(pow((float)j,2) + pow((float)i,2)))/(float)brushSize)-0.5f)*p_power)));
			}
		}

		float* test = new float[m_maxZ*m_maxX];
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, test);
		
		for(unsigned i = 0; i < plumsPos.size(); ++i)
		{
			plumsPos[i].z += test[(int)plumsPos[i].x + m_maxX * (int)plumsPos[i].y];
			glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)plumsPos[i].x, (GLint)plumsPos[i].y, 1, 1, GL_RED, GL_FLOAT, &plumsPos[i].z);
		}

		m_computeJob.m_textures[1]->Unbind(0);
		delete test;
		g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Disturb samples %d", plumsPos.size());
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
		m_context->m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Starting earthquake to create water movement");
		for(int i = 0; i < 500; ++i)
		{ 
			int x = (3 + rand() % ((m_gridSize-4)*(int)m_scale)) - (m_gridSize/2)*(int)m_scale;
			int z = (3 + rand() % ((m_gridSize-4)*(int)m_scale)) - (m_gridSize/2)*(int)m_scale;

			Disturb((float)x, (float)z, 0.001f);
		}
		m_context->m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Water is now moving");
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

	void WaterSystem::SetWaterHeight( float p_height )
	{
		m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_world->GetTagManager()->GetEntityByTag("Water"))->m_position.y = p_height;
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

	void WaterSystem::ResetWater()
	{
		float* nada = new float[m_maxX*m_maxZ]();
		m_computeJob.m_textures[1]->Bind(0);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_maxX, m_maxZ, GL_RED, GL_FLOAT, nada);
		m_computeJob.m_textures[1]->Unbind(0);
		m_computeJob.m_textures[0]->Bind(0);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_maxX, m_maxZ, GL_RED, GL_FLOAT, nada);
		m_computeJob.m_textures[0]->Unbind(0);
		delete nada;
		g_engineContext.m_logger->LogText(LogTag::WATER, LogLevel::DEBUG_PRINT, "Water has been reset!");
	}

}
