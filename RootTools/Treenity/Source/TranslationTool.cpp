#include <RootTools/Treenity/Include/TranslationTool.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/Transform.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootEngine/InputManager/Include/InputManager.h>

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
}

TranslationTool::~TranslationTool()
{
}

void TranslationTool::LoadResources(ECS::World* p_world)
{
	m_world = p_world;
}

bool TranslationTool::Pick(const glm::vec3& p_cameraPos, const glm::vec3& p_ray)
{
	if(m_selectedEntity == nullptr)
		return false;

	RootEngine::InputManager::KeyState::KeyState leftMouseButtonState = g_engineContext.m_inputSys->GetKeyState(RootEngine::InputManager::MouseButton::LEFT);

	if(m_selectedAxis == TranslationAxis::AXIS_NONE)
		m_hoverAxis = RayVsAxis(p_cameraPos, p_ray);
	
	if(leftMouseButtonState == RootEngine::InputManager::KeyState::DOWN_EDGE)
	{
		//Set selected axis when clicked
		m_selectedAxis = m_hoverAxis;
		if(m_selectedAxis != TranslationAxis::AXIS_NONE)
			return true;
	}
	else if(leftMouseButtonState == RootEngine::InputManager::KeyState::DOWN && m_selectedAxis != TranslationAxis::AXIS_NONE )
	{
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
	if(m_visible)
	{
		//X
		if(m_hoverAxis == TranslationAxis::AXIS_X)
			g_engineContext.m_renderer->AddLine(p_position, p_position + glm::vec3(m_axisLength, 0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
		else
			g_engineContext.m_renderer->AddLine(p_position, p_position + glm::vec3(m_axisLength, 0.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		//Y
		if(m_hoverAxis == TranslationAxis::AXIS_Y)
			g_engineContext.m_renderer->AddLine(p_position, p_position + glm::vec3(0.0f, m_axisLength, 0.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
		else
			g_engineContext.m_renderer->AddLine(p_position, p_position + glm::vec3(0.0f, m_axisLength, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		//Z
		if(m_hoverAxis == TranslationAxis::AXIS_Z)
			g_engineContext.m_renderer->AddLine(p_position, p_position + glm::vec3(0.0f, 0.0f, m_axisLength), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
		else
			g_engineContext.m_renderer->AddLine(p_position, p_position + glm::vec3(0.0f, 0.0f, m_axisLength), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	}
}

void TranslationTool::Hide()
{
	Tool::Hide();
}

void TranslationTool::Show()
{
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

	switch (m_selectedAxis)
	{
	case TranslationAxis::AXIS_X: 
		{
			glm::vec3 camPos = glm::vec3(selectedPos.x, p_camPos.y, p_camPos.z);
			PointOnPlane pointOnPlane = GetPointOnPlane(camPos, p_camPos, p_ray);
			if(pointOnPlane.Hit)
				selectedTrans->m_position.x = pointOnPlane.Point.x - GetDragOffset(pointOnPlane.Point.x, selectedTrans->m_position.x);
		}
		break;
	case TranslationAxis::AXIS_Y: 
		{
			glm::vec3 camPos = glm::vec3(p_camPos.x, selectedPos.y, p_camPos.z);
			PointOnPlane pointOnPlane = GetPointOnPlane(camPos, p_camPos, p_ray);
			if(pointOnPlane.Hit)
				selectedTrans->m_position.y = pointOnPlane.Point.y - GetDragOffset(pointOnPlane.Point.y, selectedTrans->m_position.y);
		}
		break;
	case TranslationAxis::AXIS_Z: 
		{
			glm::vec3 camPos = glm::vec3(p_camPos.x, p_camPos.y, selectedPos.z);
			PointOnPlane pointOnPlane = GetPointOnPlane(camPos, p_camPos, p_ray);
			if(pointOnPlane.Hit)
				selectedTrans->m_position.z = pointOnPlane.Point.z - GetDragOffset(pointOnPlane.Point.z, selectedTrans->m_position.z);
		}
		break;
	default:
		break;
	}
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
		float collisionDistance = CheckRayVsAABB(p_ray, p_cameraPos, toolPos + m_axisAABB[i].m_lower, toolPos + m_axisAABB[i].m_upper);
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
