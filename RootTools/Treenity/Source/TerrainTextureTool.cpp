#include <RootTools/Treenity/Include/TerrainTextureTool.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/InputManager/Include/InputManager.h>

extern RootEngine::GameSharedContext g_engineContext;

TerrainTextureTool::TerrainTextureTool()
	: Tool(), m_timer(0.0f)
{
}

TerrainTextureTool::~TerrainTextureTool()
{

}

void TerrainTextureTool::LoadResources( ECS::World* p_world )
{
	m_world = p_world;
}

bool TerrainTextureTool::Pick( const glm::vec3& p_cameraPos, const glm::vec3& p_ray )
{
	//Get entity with tag Terrian
	ECS::Entity* terrainEnt = m_world->GetTagManager()->GetEntityByTag("Terrain");
	if(terrainEnt == nullptr)
	{
		g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::WARNING, "No terrain entity found!");
		return false;
	}

	m_terrainTrans = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(terrainEnt);
	if(m_terrainTrans == nullptr)
	{
		g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::WARNING, "Terrain entity has no transform!");
		return false;
	}

	RootForce::Renderable* terrainRender = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(terrainEnt);
	if(terrainRender == nullptr)
	{
		g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::WARNING, "Terrain entity has no renderable!");
		return false;
	}

	RootEngine::Model* terrainModel = terrainRender->m_model;
	if(terrainModel == nullptr)
	{
		g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::WARNING, "Terrain entity has no model!");
		return false;
	}

	Render::MeshInterface* terrainMesh = terrainModel->m_meshes[0];
	if(terrainMesh == nullptr)
	{
		g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::WARNING, "Terrain entity has no mesh[0]!");
		return false;
	}

	Render::BufferInterface* terrainVBO = terrainMesh->GetVertexBuffer();
	if(terrainVBO == nullptr)
	{
		g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::WARNING, "Terrain entity has no VBO!");
		return false;
	}

	RootEngine::InputManager::KeyState::KeyState leftMouseButtonState = g_engineContext.m_inputSys->GetKeyState(RootEngine::InputManager::MouseButton::LEFT);
	RootEngine::InputManager::KeyState::KeyState rightMouseButtonState = g_engineContext.m_inputSys->GetKeyState(RootEngine::InputManager::MouseButton::RIGHT);
	if(leftMouseButtonState == RootEngine::InputManager::KeyState::DOWN_EDGE)
	{
		m_timer = 1.0f/32.0f;
	}

	unsigned numberOfAttribs = terrainMesh->GetVertexAttribute()->GetNumAttributes();
	m_numElements = terrainVBO->GetNumElements();
		
	/*if(numberOfAttribs != 3) 
	{
		g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::WARNING, "Number of attribs is %d", numberOfAttribs);
		return false;
	}*/

	//Set as memebers for the rest of the methods to use
	m_vertexData = static_cast<Render::Vertex1P1N1UV*>(terrainVBO->MapBuffer(GL_READ_WRITE));
	m_width = glm::sqrt(m_numElements);

		
	if(m_vertexData == nullptr)
	{
		g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::WARNING, "Terrain tool could not map buffer!");
		return false;
	}

	glm::ivec2 terrainHitPos = GetRayMarchCollision(p_cameraPos, p_ray);

	if(!IsCoordInsideTerrain(terrainHitPos))
	{
		if(terrainVBO->UnmapBuffer() == false)
			g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::WARNING, "Terrain tool could not unmap buffer!");

		return false;
	}

	//Testing brush material
	m_terrainModelMatrix = glm::translate(glm::mat4(1.0f), m_terrainTrans->m_interpolatedPosition);
	m_terrainModelMatrix = glm::rotate(m_terrainModelMatrix, m_terrainTrans->m_orientation.GetAngle(), m_terrainTrans->m_orientation.GetAxis());
	m_terrainModelMatrix = glm::scale(m_terrainModelMatrix, m_terrainTrans->m_scale);

	int texSize = 0;
	Render::TextureInterface* blendMapTexture = nullptr;

	//Find blendmap
	auto blendTex = terrainRender->m_material->m_textures.find(Render::TextureSemantic::TEXTUREMAP);
	if(blendTex != terrainRender->m_material->m_textures.end())
	{
		//Found blendmap and it is not null
		if(blendTex->second != nullptr)
		{
			blendMapTexture = blendTex->second;

			texSize = (int)blendTex->second->GetWidth();
		}
	}

	//Get texture coords
	float textureSizeFactor = (float)texSize / (float)m_width;

	//Flip x and z(y)
	glm::ivec2 textureHitPos = glm::ivec2(textureSizeFactor * glm::vec2(terrainHitPos.y, terrainHitPos.x));

	m_shaderdata.y = 0.0f;
	m_shaderdata.w = m_editorInterface->GetTerrainTextureBrush()->GetSize() * m_terrainTrans->m_scale.x / textureSizeFactor; // divide by texture size factor to display a correct brush
			
	Render::RenderJob job;	
	job.m_mesh = terrainMesh;
	job.m_material = g_engineContext.m_renderer->CreateMaterial(m_editorInterface->GetTerrainTextureBrush()->GetActiveMaterial());
	job.m_forward = terrainRender->m_forward;
	job.m_refractive = terrainRender->m_refractive;
	job.m_params[Render::Semantic::MODEL] = &m_terrainModelMatrix;
	job.m_params[Render::Semantic::COLOR] = &m_shaderdata;
	job.m_params[Render::Semantic::TRANSPOSITION] = &m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_world->GetTagManager()->GetEntityByTag("Camera"))->m_position;
	job.m_renderPass = RootForce::RenderPass::RENDERPASS_EDITOR;
	job.m_position = m_terrainTrans->m_interpolatedPosition;

	g_engineContext.m_renderer->AddRenderJob(job);

	if(leftMouseButtonState == RootEngine::InputManager::KeyState::DOWN)
	{
		m_timer += m_world->GetDelta();
		m_shaderdata.y = 1.0f;
		float updateFreq = 1.0f/32.0f;
		if(m_timer >= updateFreq)
		{
			m_timer = m_timer - updateFreq;
		
			if(blendMapTexture != nullptr)
			{
				unsigned char* textureData = new unsigned char[texSize*texSize*4];

				blendTex->second->Bind(0);
				//m_context->m_profiler->BeginGPUTimer();
				glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
				//double time = m_context->m_profiler->EndGPUTimer();
				//m_context->m_logger->LogText(LogTag::RENDER, LogLevel::DEBUG_PRINT, "Compute time: %f ms", time);

				//Buffer is mapped, ready to be read and written to
				for (unsigned i = 0; i < m_editorInterface->GetTerrainTextureBrush()->GetCurrentBrushShapeData()->size(); ++i)
				{
					glm::ivec2 brushLocalPos	= m_editorInterface->GetTerrainTextureBrush()->GetCurrentBrushShapeData()->at(i).pos;
					float strength				= m_editorInterface->GetTerrainTextureBrush()->GetCurrentBrushShapeData()->at(i).strength;

					float colori[]  = {40, 0, 0, 0};
					
					glm::ivec2 pos2d = textureHitPos + brushLocalPos;

					//Check out of range
					if(pos2d.x <= 0 || pos2d.y <= 0 || pos2d.x >= texSize || pos2d.y >= texSize)
						continue;

					unsigned char color[]  = {0, 0, 0, 0};
					color[0] = c_clamp(colori[0] += (float)textureData[pos2d.x * 4 + texSize * 4 * pos2d.y], 0, 255);
					color[1] = c_clamp(colori[1] += (float)textureData[pos2d.x * 4 + texSize * 4 * pos2d.y + 1], 0, 255);
					color[2] = c_clamp(colori[2] += (float)textureData[pos2d.x * 4 + texSize * 4 * pos2d.y + 2], 0, 255);
					color[3] = c_clamp(colori[3] += (float)textureData[pos2d.x * 4 + texSize * 4 * pos2d.y + 3], 0, 255);
					
					glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)pos2d.x, (GLint)pos2d.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &color[0]);
				}
				//Clean up
				delete[] textureData;
			}

			if(terrainVBO->UnmapBuffer() == false)
				g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::WARNING, "Terrain tool could not unmap buffer!");

			return true;
		}

	}

	if(terrainVBO->UnmapBuffer() == false)
		g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::WARNING, "Terrain tool could not unmap buffer!");

	return false;
}

void TerrainTextureTool::SetPosition( const glm::vec3& p_position )
{

}

void TerrainTextureTool::Hide()
{
	Tool::Hide();
}

void TerrainTextureTool::Show()
{
	Tool::Show();
}

int TerrainTextureTool::GetVertexPosition( const glm::ivec2& p_pos )
{
	int pos = p_pos.y * m_width + p_pos.x;

	if(IsCoordInsideTerrain(p_pos))
		return pos;
	else
		return -1;
}

glm::ivec2 TerrainTextureTool::GetRayMarchCollision( const glm::vec3& p_cameraPos, const glm::vec3& p_ray )
{
	glm::vec3 rayPos = p_cameraPos;
	glm::vec3 rayDir = glm::normalize(p_ray);
	glm::ivec2 hitPos;

	float midTransFactorX = (m_terrainTrans->m_scale.x * m_width) / 2.0f;
	float midTransFactorZ = (m_terrainTrans->m_scale.z * m_width) / 2.0f;

	int rayDistCounter = 0;
	int iterations = 5000;
	while(rayDistCounter < iterations)
	{
		hitPos = glm::ivec2((rayPos.z - m_terrainTrans->m_position.z + midTransFactorZ) / m_terrainTrans->m_scale.z, (rayPos.x - m_terrainTrans->m_position.x + midTransFactorX) / m_terrainTrans->m_scale.x);
		if(IsCoordInsideTerrain(hitPos))
		{
			if(rayPos.y <= m_vertexData[GetVertexPosition(hitPos)].m_pos.y + m_terrainTrans->m_position.y)
			{
				break;
			}
		}
		rayPos += rayDir;
		m_shaderdata.x = rayPos.x;
		m_shaderdata.z = rayPos.z;
		rayDistCounter++;
	}

	if(rayDistCounter == iterations)
		return glm::ivec2(9999999, 9999999);
	else
		return hitPos;
}

bool TerrainTextureTool::IsCoordInsideTerrain( glm::ivec2 p_pos)
{
	if((p_pos.x >= 0 && p_pos.x < m_width) && (p_pos.y >= 0 && p_pos.y < m_width))
		return true;
	else 
		return false;
}

unsigned char TerrainTextureTool::c_clamp(float x, float a, float b)
{
	if (x > b)
		return (unsigned char)b;
	else if(x < a)
		return (unsigned char)a;
	else 
		return (unsigned char)x;
}