#include <RootTools/Treenity/Include/TerrainTool.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/InputManager/Include/InputManager.h>

extern RootEngine::GameSharedContext g_engineContext;

TerrainTool::TerrainTool()
	: Tool(), m_timer(0.0f), m_plateauHeight(0.0f), m_downEdge(false)
{
}

TerrainTool::~TerrainTool()
{

}

void TerrainTool::LoadResources( ECS::World* p_world )
{
	m_world = p_world;
}

bool TerrainTool::Pick( const glm::vec3& p_cameraPos, const glm::vec3& p_ray )
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
		m_downEdge = true;
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
			
	m_shaderdata.y = 0.0f;
	m_shaderdata.w = m_editorInterface->GetTerrainGeometryBrush()->GetSize() * m_terrainTrans->m_scale.x;
			
	Render::RenderJob job;	
	job.m_mesh = terrainMesh;
	job.m_material = g_engineContext.m_renderer->CreateMaterial(m_editorInterface->GetTerrainGeometryBrush()->GetActiveMaterial());
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
		
			
			//Buffer is mapped, ready to be read and written to
			//If not smooth only, run the height calculation algorithm
			if(!m_editorInterface->GetTerrainGeometryBrush()->GetSmoothOnly())
				for (unsigned i = 0; i < m_editorInterface->GetTerrainGeometryBrush()->GetCurrentBrushShapeData()->size(); ++i)
				{
					glm::ivec2 brushLocalPos	= m_editorInterface->GetTerrainGeometryBrush()->GetCurrentBrushShapeData()->at(i).pos;
					float strength				= m_editorInterface->GetTerrainGeometryBrush()->GetCurrentBrushShapeData()->at(i).strength;

					glm::ivec2 pos2d = terrainHitPos + brushLocalPos;
			
					int pos = GetVertexPosition(pos2d);

					if(pos != -1)
					{
						if(m_editorInterface->GetTerrainGeometryBrush()->GetFlat())
							m_vertexData[pos].m_pos.y +=  m_editorInterface->GetTerrainGeometryBrush()->GetStrength();
						else if(m_editorInterface->GetTerrainGeometryBrush()->GetPlateau())
							m_vertexData[pos].m_pos.y = m_editorInterface->GetTerrainGeometryBrush()->GetStrength() + m_plateauHeight;
						else
							m_vertexData[pos].m_pos.y += strength * m_editorInterface->GetTerrainGeometryBrush()->GetStrength();
					}
				}
			//If autosmooth or smooth only, run the smoothing algorithm
			if(m_editorInterface->GetTerrainGeometryBrush()->GetSmoothOnly() || m_editorInterface->GetTerrainGeometryBrush()->GetAutoSmooth())
				for (unsigned i = 0; i < m_editorInterface->GetTerrainGeometryBrush()->GetCurrentBrushShapeData()->size(); ++i)
				{
					glm::ivec2 brushLocalPos = m_editorInterface->GetTerrainGeometryBrush()->GetCurrentBrushShapeData()->at(i).pos;

					glm::ivec2 pos2d = terrainHitPos + brushLocalPos;

					int pos = GetVertexPosition(pos2d);

					if(pos != -1)
					{
						float smoothHeight = AverageHeight(pos2d);
						m_vertexData[pos].m_pos.y = smoothHeight;
					}

				}

			for (unsigned i = 0; i < m_editorInterface->GetTerrainGeometryBrush()->GetCurrentNormalBrushShapeData()->size(); ++i)
			{
				glm::ivec2 brushLocalPos	= m_editorInterface->GetTerrainGeometryBrush()->GetCurrentNormalBrushShapeData()->at(i).pos;

				glm::ivec2 pos2d = terrainHitPos + brushLocalPos;

				int pos = GetVertexPosition(pos2d);

				if(pos != -1)
				{
					m_vertexData[pos].m_normal = CalcNormalOnCoord(pos2d);
				}
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

void TerrainTool::SetPosition( const glm::vec3& p_position )
{

}

void TerrainTool::Hide()
{
	Tool::Hide();
}

void TerrainTool::Show()
{
	Tool::Show();
}

int TerrainTool::GetVertexPosition( const glm::ivec2& p_pos )
{
	int pos = p_pos.y * m_width + p_pos.x;

	if(IsCoordInsideTerrain(p_pos))
		return pos;
	else
		return -1;
}

float TerrainTool::AverageHeight( const glm::ivec2& p_pos)
{
	float avg = 0.0f;
	float num = 0.0f;

	for(int m = p_pos.x - 1; m <= p_pos.x + 1; ++m)
	{ 
		for(int n = p_pos.y - 1; n <= p_pos.y + 1; ++n)
		{
			//Check if in bounds
			if(IsCoordInsideTerrain(glm::ivec2(m, n)))
			{
				avg += m_vertexData[GetVertexPosition(glm::ivec2(m, n))].m_pos.y;
				num += 1.0f;
			}
		}
	}
	return avg / num;
}

glm::ivec2 TerrainTool::GetRayMarchCollision( const glm::vec3& p_cameraPos, const glm::vec3& p_ray )
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
	{
		if(m_downEdge)
		{
			m_plateauHeight = 0.0f;
			m_downEdge = false;
		}
		return glm::ivec2(9999999, 9999999);
	}
	else
	{
		if(m_downEdge)
		{
			m_plateauHeight = m_vertexData[GetVertexPosition(hitPos)].m_pos.y;
			m_downEdge = false;
		}
		return hitPos;
	}
}

bool TerrainTool::IsCoordInsideTerrain( glm::ivec2 p_pos)
{
	if((p_pos.x >= 0 && p_pos.x < m_width) && (p_pos.y >= 0 && p_pos.y < m_width))
		return true;
	else 
		return false;
}

glm::vec3 TerrainTool::CalcNormalOnCoord( const glm::ivec2& p_pos )
{
	// Estimate normals for interior nodes using central difference.
	float invTwoDX = 1.0f / (2.0f * m_terrainTrans->m_scale.x);
	float invTwoDZ = 1.0f / (2.0f * m_terrainTrans->m_scale.z);

	glm::ivec2 normalPos[] = 
	{
		glm::clamp(p_pos + glm::ivec2(-1, 0), glm::ivec2(0, 0), glm::ivec2(m_width-1, m_width-1)),
		glm::clamp(p_pos + glm::ivec2(1, 0), glm::ivec2(0, 0), glm::ivec2(m_width-1, m_width-1)),
		glm::clamp(p_pos + glm::ivec2(0, -1), glm::ivec2(0, 0), glm::ivec2(m_width-1, m_width-1)),
		glm::clamp(p_pos + glm::ivec2(0, 1), glm::ivec2(0, 0), glm::ivec2(m_width-1, m_width-1))
	};

	float t = m_vertexData[GetVertexPosition(normalPos[0])].m_pos.y;
	float b = m_vertexData[GetVertexPosition(normalPos[1])].m_pos.y;
	float l = m_vertexData[GetVertexPosition(normalPos[2])].m_pos.y;
	float r = m_vertexData[GetVertexPosition(normalPos[3])].m_pos.y;

	glm::vec3 tanZ(0.0f, (b-t)*invTwoDZ, 1.0f);
	glm::vec3 tanX(1.0f, (r-l)*invTwoDX, 0.0f);

	return  glm::normalize(glm::cross(tanZ, tanX));
}
