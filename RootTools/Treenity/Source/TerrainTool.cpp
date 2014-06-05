#include <RootTools/Treenity/Include/TerrainTool.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/InputManager/Include/InputManager.h>

extern RootEngine::GameSharedContext g_engineContext;

TerrainTool::TerrainTool()
	: Tool()
{
	m_brushManager.GetCurrentBrush()->SetSize(10);
	m_brushManager.GetCurrentBrush()->SetStrength(7);
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
	ECS::Entity* terrainEnt = m_world->GetTagManager()->GetEntityByTag("Water");
	if(terrainEnt == nullptr)
	{
		g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::WARNING, "No terrain entity found!");
		return false;
	}

	RootForce::Transform* terrainTrans = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(terrainEnt);
	if(terrainTrans == nullptr)
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

	if(leftMouseButtonState == RootEngine::InputManager::KeyState::DOWN)
	{
		unsigned numberOfAttribs = terrainMesh->GetVertexAttribute()->GetNumAttributes();
		m_numElements = terrainVBO->GetNumElements();
		
		/*if(numberOfAttribs != 3) 
		{
			g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::WARNING, "Number of attribs is %d", numberOfAttribs);
			return false;
		}*/

		//Set as memebers for the rest of the methods to use
		m_vertexData = static_cast<Render::Vertex1P1N1UV1T1BT*>(terrainVBO->MapBuffer(GL_READ_WRITE));
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

		//Buffer is mapped, ready to be read and written to
		for (unsigned i = 0; i < m_brushManager.GetCurrentBrush()->GetBrush()->size(); ++i)
		{
			glm::ivec2 brushLocalPos	= m_brushManager.GetCurrentBrush()->GetBrush()->at(i).pos;
			float strength				= m_brushManager.GetCurrentBrush()->GetBrush()->at(i).strength;

			glm::ivec2 pos2d = terrainHitPos + brushLocalPos;
			
			int pos = GetVertexPosition(pos2d);

			if(pos != -1)
				m_vertexData[pos].m_pos.y += strength;
		}

		for (unsigned i = 0; i < m_brushManager.GetCurrentBrush()->GetBrush()->size(); ++i)
		{
			glm::ivec2 brushLocalPos	= m_brushManager.GetCurrentBrush()->GetBrush()->at(i).pos;

			glm::ivec2 pos2d = terrainHitPos + brushLocalPos;

			int pos = GetVertexPosition(pos2d);

			if(pos != -1)
			{
				float smoothHeight = AverageHeight(pos2d);
				m_vertexData[pos].m_pos.y = smoothHeight;
			}

		}

		if(terrainVBO->UnmapBuffer() == false)
			g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::WARNING, "Terrain tool could not unmap buffer!");

		return true;
	}

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

	if(pos >= m_numElements || pos < 0)
		return -1;
	else
		return pos;
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
			if( m * n < m_numElements || m * n >= 0)
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

	int rayDistCounter = 0;
	while(rayDistCounter < 2000)
	{
		if(IsCoordInsideTerrain(glm::ivec2(rayPos.z/40.0f, rayPos.x/40.0f)))
		{
			if(rayPos.y <= m_vertexData[GetVertexPosition(glm::ivec2(rayPos.z/40.0f, rayPos.x/40.0f))].m_pos.y)
			{
				g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "HIT TERRAIN");
				break;
			}
		}
		rayPos += p_ray;
		rayDistCounter++;
	}

	return glm::ivec2(rayPos.z/40.0f, rayPos.x/40.0f);
}

bool TerrainTool::IsCoordInsideTerrain( glm::ivec2 p_pos)
{
	if((p_pos.x >= 0 && p_pos.x < m_width) && (p_pos.y >= 0 && p_pos.y < m_width))
		return true;
	else 
		return false;
}

