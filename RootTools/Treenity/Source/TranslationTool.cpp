#include <RootTools/Treenity/Include/TranslationTool.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/Transform.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootEngine/InputManager/Include/InputManager.h>
#include <RootTools/Treenity/Include/Picking.h>

extern RootEngine::GameSharedContext g_engineContext;

TranslationTool::TranslationTool()
	:Tool(),
	m_firstDrag(true), 
	m_dragOffset(0.0f), 
	m_selectedAxis(TranslationAxis::AXIS_NONE), 
	m_hoverAxis(TranslationAxis::AXIS_NONE),
	m_axisLength(1.0f)
{
	m_axisAABB[0] = AxisBoundingBox(glm::vec3(0.1f, -0.1f, -0.1f), glm::vec3(m_axisLength, 0.1f, 0.1f));//X
	m_axisAABB[1] = AxisBoundingBox(glm::vec3(-0.1f, 0.1f, -0.1f), glm::vec3(0.1f, m_axisLength, 0.1f));//Y
	m_axisAABB[2] = AxisBoundingBox(glm::vec3(-0.1f, -0.1f, 0.1f), glm::vec3(0.1f, 0.1f, m_axisLength));//Z

	m_axisOBB[0] = RootForce::OBB(0.1f, m_axisLength, -0.1f, 0.1f, -0.1f, 0.1f, glm::mat4(1.0f));
	m_axisOBB[1] = RootForce::OBB(-0.1f, 0.1f, 0.1f, m_axisLength, -0.1f, 0.1f, glm::mat4(1.0f));
	m_axisOBB[2] = RootForce::OBB(-0.1f, 0.1f, -0.1f, 0.1f, 0.1f, m_axisLength, glm::mat4(1.0f));
}

TranslationTool::~TranslationTool()
{
}

void TranslationTool::LoadResources(ECS::World* p_world)
{
	m_world = p_world;

	m_toolEntityMaterial = g_engineContext.m_renderer->CreateMaterial("Axes");
	m_toolEntityMaterial->m_effect = g_engineContext.m_resourceManager->LoadEffect("Translation");
	
	m_toolEntity = m_world->GetEntityManager()->CreateEntity();
	RootForce::Renderable* r = m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(m_toolEntity);
	r->m_model = g_engineContext.m_resourceManager->LoadCollada("GUI/axes");
	r->m_material = m_toolEntityMaterial;
	r->m_material->m_textures[Render::TextureSemantic::DIFFUSE] = g_engineContext.m_resourceManager->LoadTexture("GUI/axes", Render::TextureType::TEXTURE_2D);
	

	m_world->GetTagManager()->RegisterEntity("TranslationTool", m_toolEntity);
	m_world->GetGroupManager()->RegisterEntity("NonExport", m_toolEntity);
}

bool TranslationTool::Pick(const glm::vec3& p_cameraPos, const glm::vec3& p_ray)
{
	if(m_selectedEntity == nullptr)
		return false;

	RootForce::Transform* selectedTrans =  m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_selectedEntity);

	RootEngine::InputManager::KeyState::KeyState leftMouseButtonState = g_engineContext.m_inputSys->GetKeyState(RootEngine::InputManager::MouseButton::LEFT);
	RootEngine::InputManager::KeyState::KeyState rightMouseButtonState = g_engineContext.m_inputSys->GetKeyState(RootEngine::InputManager::MouseButton::RIGHT);

	if(m_selectedAxis == TranslationAxis::AXIS_NONE)
		m_hoverAxis = RayVsAxis(p_cameraPos, p_ray);
	
	if(leftMouseButtonState == RootEngine::InputManager::KeyState::DOWN_EDGE)
	{
		m_selectedAxis = m_hoverAxis;

		switch(m_selectedAxis)
		{
		case TranslationAxis::AXIS_X:
			{
				if(m_editorInterface->GetToolMode() == ToolMode::LOCAL)
				{
					m_axis = -selectedTrans->m_orientation.GetRight();
				}
				else
				{
					m_axis = glm::vec3(1,0,0);
				}
			}
			break;
			
		case TranslationAxis::AXIS_Y:
			{
				if(m_editorInterface->GetToolMode() == ToolMode::LOCAL)
				{
					m_axis = selectedTrans->m_orientation.GetUp();
				}
				else
				{
					m_axis = glm::vec3(0,1,0);
				}
			}
			break;
			
		case TranslationAxis::AXIS_Z:
			{
				if(m_editorInterface->GetToolMode() == ToolMode::LOCAL)
				{
					m_axis = selectedTrans->m_orientation.GetFront();
				}
				else
				{
					m_axis = glm::vec3(0,0,1);
				}
			}
			break;
		};

		glm::vec3 cameraDir = glm::normalize(selectedTrans->m_position - p_cameraPos); 
		glm::vec3 b = glm::normalize(glm::cross(cameraDir, m_axis));
		m_pickedPlaneNormal = glm::cross(b, m_axis);

		//g_engineContext.m_renderer->AddLine(selectedTrans->m_position, selectedTrans->m_position + m_axis * 10.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		//g_engineContext.m_renderer->AddLine(selectedTrans->m_position, selectedTrans->m_position + b * 10.0f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		//g_engineContext.m_renderer->AddLine(selectedTrans->m_position, selectedTrans->m_position + m_pickedPlaneNormal * 10.0f, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

		float t = 999999.0f;
		RayVsPlane(selectedTrans->m_position, m_pickedPlaneNormal, p_cameraPos, p_ray, t);
		glm::vec3 hit = p_cameraPos + t * p_ray;		
		m_dist0 = glm::dot(hit, m_axis);

		m_position_t0 = selectedTrans->m_position;

		if(m_selectedAxis != TranslationAxis::AXIS_NONE)
			return true;
	}
	else if(leftMouseButtonState == RootEngine::InputManager::KeyState::DOWN && m_selectedAxis != TranslationAxis::AXIS_NONE )
	{
		if(rightMouseButtonState == RootEngine::InputManager::KeyState::DOWN_EDGE)
		{
			m_selectedAxis = TranslationAxis::AXIS_NONE;
			selectedTrans->m_position = m_position_t0;
			return true;
		}
		//If mouse is pressed and there is a selection -> drag away
		Drag(p_ray, p_cameraPos);
		return true;
	}
	else if(leftMouseButtonState == RootEngine::InputManager::KeyState::UP_EDGE && m_selectedAxis != TranslationAxis::AXIS_NONE)
	{
		//Set AXIS_NONE when releasing the mouse button
		m_selectedAxis = TranslationAxis::AXIS_NONE;
		return true;
	}
	return false;
}

void TranslationTool::SetPosition(const glm::vec3& p_position)
{
	RootForce::Transform* toolTransform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_toolEntity);
	RootForce::Transform* selectedTransform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_selectedEntity);

	if(m_visible)
	{
		m_transformMatrix = glm::translate(glm::mat4(1.0f), toolTransform->m_position);
		if(m_editorInterface->GetToolMode() == ToolMode::LOCAL)
		{
			m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_toolEntity)->m_orientation = selectedTransform->m_orientation;
			m_transformMatrix = glm::rotate(m_transformMatrix, selectedTransform->m_orientation.GetAngle(), selectedTransform->m_orientation.GetAxis());
		}
		else
		{
			m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_toolEntity)->m_orientation = RootForce::Orientation();
		}

		m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_toolEntity)->m_position = p_position;

		//Debug(&m_axisOBB[0], m_transformMatrix, glm::vec3(1,0,0));
		//Debug(&m_axisOBB[1], m_transformMatrix, glm::vec3(1,0,0));
		//Debug(&m_axisOBB[2], m_transformMatrix, glm::vec3(1,0,0));
	}
}

void TranslationTool::Hide()
{
	m_world->GetEntityManager()->RemoveComponent<RootForce::Transform>(m_toolEntity);

	Tool::Hide();
}

void TranslationTool::Show()
{
	m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(m_toolEntity);

	Tool::Show();
}

TranslationTool::PointOnPlane TranslationTool::GetPointOnPlane( const glm::vec3& p_camPos, const glm::vec3& p_worldCamPos, const glm::vec3& p_rayDir  )
{
	//p_camPos is the camera position where one of the axes has been set to the same at the selected entity. The axis is the one selected.
	float t;
	bool hit = true;

	glm::vec3 selectedPos = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_selectedEntity)->m_position;

	glm::vec3 normalFlip = glm::normalize(selectedPos - p_camPos);

	float denom = glm::dot(normalFlip, (selectedPos - p_worldCamPos));

	if(denom > 1e-6)
	{
		t = denom / glm::dot(normalFlip, p_rayDir);
		if(t < 0.0f)
		{
			//m_context->m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "t < 0.0f");
			hit = false; 
		}
	}
	else
		hit = false; 

	return PointOnPlane(p_worldCamPos + (p_rayDir * t), hit);
}

float TranslationTool::GetDragOffset( float p_pointOnAxis, float p_pointOfEntity )
{
	if(m_firstDrag)
	{
		m_dragOffset = p_pointOnAxis - p_pointOfEntity;
		m_firstDrag = false;
	}
	return m_dragOffset;

}

float TranslationTool::CheckRayVsAABB(const glm::vec3& p_rayDir, const glm::vec3& p_rayOrigin, const glm::vec3& p_bound1, const glm::vec3& p_bound2 )
{
	if(m_selectedEntity == nullptr)
		return -1.0f;

	glm::vec3 invdir = 1.0f / p_rayDir;

	float t1 = (p_bound1.x - p_rayOrigin.x)*invdir.x;
	float t2 = (p_bound2.x - p_rayOrigin.x)*invdir.x;
	float t3 = (p_bound1.y - p_rayOrigin.y)*invdir.y;
	float t4 = (p_bound2.y - p_rayOrigin.y)*invdir.y;
	float t5 = (p_bound1.z - p_rayOrigin.z)*invdir.z;
	float t6 = (p_bound2.z - p_rayOrigin.z)*invdir.z;

	float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
	float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

	if(tmax < 0)
		return -1.0f;
	if(tmin > tmax)
		return -1.0f;

	return tmin;
}

void TranslationTool::Drag( const glm::vec3& p_ray, const glm::vec3& p_camPos)
{
	if(m_selectedEntity == nullptr)
		return;

	RootForce::Transform* selectedTrans =  m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_selectedEntity);
	glm::vec3 selectedPos = selectedTrans->m_position;

	float t = 999999.0f;
	RayVsPlane(selectedPos, m_pickedPlaneNormal, p_camPos, p_ray, t);
	glm::vec3 hit = p_camPos + t * p_ray;
			
	float distDelta = m_dist0 - glm::dot(hit, m_axis);

	selectedTrans->m_position = m_position_t0 - distDelta * m_axis;
}

TranslationAxis::TranslationAxis TranslationTool::RayVsAxis( const glm::vec3& p_cameraPos, const glm::vec3& p_ray )
{
	float closestDist = 999999.0f;
	//Set default to AXIS_NONE, if no axit is hit, then this is the outcome
	TranslationAxis::TranslationAxis selectedAxis = TranslationAxis::AXIS_NONE;

	//Position of axis
	ECS::Entity* cameraEntity = m_world->GetTagManager()->GetEntityByTag("Camera"); 
	glm::vec3 cameraPos = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(cameraEntity)->m_position;

	RootForce::Transform* cameraTransform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(cameraEntity);
	RootForce::Transform* selectedTransform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_selectedEntity);

	glm::vec3 dir = -glm::normalize(cameraPos - selectedTransform->m_position);
	float t = 8.0f / glm::dot(cameraTransform->m_orientation.GetFront(), dir);
	glm::vec3 toolPos = cameraPos + t * dir;

	//Test Axis selection
	for(int i = 0; i < 3; ++i)
	{
		float collisionDistance = 999999.0f;
		RayVsOBB(p_cameraPos, p_ray, &m_axisOBB[i], m_transformMatrix, collisionDistance);
		
		//float collisionDistance = CheckRayVsAABB(p_ray, p_cameraPos, toolPos + m_axisAABB[i].m_lower, toolPos + m_axisAABB[i].m_upper);
		if(collisionDistance < closestDist && collisionDistance >= 0.0f)
		{
			//An axis is hit! Must loop through all axis to be sure of the closet.
			selectedAxis = (TranslationAxis::TranslationAxis)i;
			closestDist = collisionDistance;
			m_firstDrag = true;
		}
	}

	return selectedAxis;
}
