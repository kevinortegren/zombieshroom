#include <RootTools/Treenity/Include/RotationTool.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/Transform.h>

#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootEngine/InputManager/Include/InputManager.h>
extern RootEngine::GameSharedContext g_engineContext;

const float RotationTool::s_pickMargin = 0.4f; // Acceptable margin when picking axis.
const glm::vec4 RotationTool::s_hightlightColor = glm::vec4(1,1,0,1); // Yellow highlight color.

RotationTool::RotationTool()
	: m_selectedAxis(RotationAxis::AXIS_NONE), m_angle0(0.0f)
{

}

RotationTool::~RotationTool()
{

}

void RotationTool::LoadResources(ECS::World* p_world)
{
	m_world = p_world;

	// Load circle material.
	m_axisMaterial = g_engineContext.m_renderer->CreateMaterial("CircleMaterial");
	m_axisMaterial->m_effect = g_engineContext.m_resourceManager->LoadEffect("Circle");
	
	// Uniform color variables for shader.
	m_axisColors[RotationAxis::AXIS_X] = glm::vec4(1,0,0,1);
	m_axisColors[RotationAxis::AXIS_Y] = glm::vec4(0,1,0,1);
	m_axisColors[RotationAxis::AXIS_Z] = glm::vec4(0,0,1,1);

	// Load circle meshes.
	// X
	m_axisEntities[RotationAxis::AXIS_X] = m_world->GetEntityManager()->CreateEntity();
	RootForce::Renderable* r = m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(m_axisEntities[RotationAxis::AXIS_X]);
	r->m_model = g_engineContext.m_resourceManager->LoadCollada("circle_x");
	r->m_material = m_axisMaterial;
	r->m_params[Render::Semantic::COLOR] = &m_axisColors[RotationAxis::AXIS_X];
	r->m_model->m_meshes[0]->SetPrimitiveType(GL_LINES);

	// Y
	m_axisEntities[RotationAxis::AXIS_Y] = m_world->GetEntityManager()->CreateEntity();
	RootForce::Renderable* r1 = m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(m_axisEntities[RotationAxis::AXIS_Y]);
	r1->m_model = g_engineContext.m_resourceManager->LoadCollada("circle_y");
	r1->m_material = m_axisMaterial;
	r1->m_params[Render::Semantic::COLOR] = &m_axisColors[RotationAxis::AXIS_Y];
	r1->m_model->m_meshes[0]->SetPrimitiveType(GL_LINES);

	// Z
	m_axisEntities[RotationAxis::AXIS_Z] = m_world->GetEntityManager()->CreateEntity();
	RootForce::Renderable* r2 = m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(m_axisEntities[RotationAxis::AXIS_Z]);
	r2->m_model = g_engineContext.m_resourceManager->LoadCollada("circle_z");
	r2->m_material = m_axisMaterial;
	r2->m_params[Render::Semantic::COLOR] = &m_axisColors[RotationAxis::AXIS_Z];
	r2->m_model->m_meshes[0]->SetPrimitiveType(GL_LINES);

	m_world->GetGroupManager()->RegisterEntity("NonExport", m_axisEntities[RotationAxis::AXIS_X]);
	m_world->GetGroupManager()->RegisterEntity("NonExport", m_axisEntities[RotationAxis::AXIS_Y]);
	m_world->GetGroupManager()->RegisterEntity("NonExport", m_axisEntities[RotationAxis::AXIS_Z]);
}

float RotationTool::GetAngleFromAxis(RotationAxis::RotationAxis axis, const glm::vec3& position)
{
	float angle = 0.0f;
	switch (axis)
	{
	case RotationAxis::AXIS_X:
		{
			glm::vec3 pnorm = glm::normalize(position);
			glm::vec3 up = m_gimbalOrientation.GetUp();
			glm::vec3 front = m_gimbalOrientation.GetFront();
			float y = glm::dot(up, pnorm);
			float z = glm::dot(front, pnorm);
			angle = atan2f(y, z);
		}
		break;
		
	case RotationAxis::AXIS_Y:
		{
			glm::vec3 pnorm = glm::normalize(position);
			glm::vec3 front = m_gimbalOrientation.GetFront();
			glm::vec3 left = -m_gimbalOrientation.GetRight();
			float z = glm::dot(front, pnorm);
			float x = glm::dot(left, pnorm);
			angle = atan2f(z, x);
		}
		break;
		
	case RotationAxis::AXIS_Z:
		{
			glm::vec3 pnorm = glm::normalize(position);
			glm::vec3 up = m_gimbalOrientation.GetUp();
			glm::vec3 left = -m_gimbalOrientation.GetRight();
			float y = glm::dot(up, pnorm);
			float x = glm::dot(left, pnorm);
			angle = atan2f(y, x);
		}
		break;

	default:
		break;
	}

	return angle;
}

bool RotationTool::RayVsPlane(const glm::vec3& center, const glm::vec3& normal, const glm::vec3& cameraPos, const glm::vec3& ray, float& t)
{
	float denom = glm::dot(normal, ray);
	glm::vec3 centerToCamera = center - cameraPos;
	t = glm::dot(centerToCamera, normal) / denom;
	return true;
}

bool RotationTool::Pick(const glm::vec3& p_cameraPos, const glm::vec3& p_ray)
{
	if(m_selectedEntity == nullptr)
		return false;

	// Transform of selected entity.
	RootForce::Transform* selectedTransform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_selectedEntity);

	// Transform of gimbal.
	RootForce::Transform* gimbalTransform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_axisEntities[RotationAxis::AXIS_X]);

	// Reset axis colors.
	m_axisColors[RotationAxis::AXIS_X] = glm::vec4(1,0,0,1);
	m_axisColors[RotationAxis::AXIS_Y] = glm::vec4(0,1,0,1);
	m_axisColors[RotationAxis::AXIS_Z] = glm::vec4(0,0,1,1);

	// Get orientation of gimbal.
	m_gimbalOrientation = gimbalTransform->m_orientation;
	m_entityOrientation = selectedTransform->m_orientation;

	glm::vec3 normalX = m_gimbalOrientation.GetRight();
	glm::vec3 normalY = m_gimbalOrientation.GetUp();
	glm::vec3 normalZ = m_gimbalOrientation.GetFront();

	// Debug render plane normals.
	//g_engineContext.m_renderer->AddLine(gimbalTransform->m_position, gimbalTransform->m_position - normalX, glm::vec4(1,0,0,1));
	//g_engineContext.m_renderer->AddLine(gimbalTransform->m_position, gimbalTransform->m_position - normalY, glm::vec4(0,1,0,1));
	//g_engineContext.m_renderer->AddLine(gimbalTransform->m_position, gimbalTransform->m_position - normalZ, glm::vec4(0,0,1,1));

	float bestDist = 999999.0f;
	RotationAxis::RotationAxis axis = RotationAxis::AXIS_NONE;

	float tx = 999999.0f;
	float ty = 999999.0f;
	float tz = 999999.0f;

	RayVsPlane(gimbalTransform->m_position, normalX, p_cameraPos, p_ray, tx);
	RayVsPlane(gimbalTransform->m_position, normalY, p_cameraPos, p_ray, ty);
	RayVsPlane(gimbalTransform->m_position, normalZ, p_cameraPos, p_ray, tz);

	glm::vec3 hits[3];
	hits[RotationAxis::AXIS_X] = p_cameraPos + tx * p_ray;
	hits[RotationAxis::AXIS_Y] = p_cameraPos + ty * p_ray;
	hits[RotationAxis::AXIS_Z] = p_cameraPos + tz * p_ray;

	float distx = glm::distance(gimbalTransform->m_position, hits[RotationAxis::AXIS_X]);
	float disty = glm::distance(gimbalTransform->m_position, hits[RotationAxis::AXIS_Y]);
	float distz = glm::distance(gimbalTransform->m_position, hits[RotationAxis::AXIS_Z]);

	// Modify rotationbased on left mouse button state.
	using namespace RootEngine::InputManager::KeyState;

	KeyState leftMouseButtonState = g_engineContext.m_inputSys->GetKeyState(RootEngine::InputManager::MouseButton::LEFT);

	if(m_selectedAxis == RotationAxis::AXIS_NONE)
	{
		if(distx < (1.0f + s_pickMargin) && distx > (1.0f - s_pickMargin))
		{
			bestDist = abs(distx-1.0f);
			axis = RotationAxis::AXIS_X;
		}		

		if(disty < (1.0f + s_pickMargin) && disty > (1.0f - s_pickMargin) && abs(disty-1.0f) < bestDist)
		{
			bestDist = abs(disty-1.0f);
			axis = RotationAxis::AXIS_Y;
		}	

		if(distz < (1.0f + s_pickMargin) && distz > (1.0f - s_pickMargin) && abs(distz-1.0f) < bestDist)
		{
			bestDist = abs(distz-1.0f);
			axis = RotationAxis::AXIS_Z;
		}	

		if(axis != RotationAxis::AXIS_NONE)
		{
			// Highlight hoovered axis.
			m_axisColors[axis] = s_hightlightColor;
		}
	}

	if(leftMouseButtonState == KeyState::DOWN_EDGE && axis != RotationAxis::AXIS_NONE)
	{
		if(distx < (1.0f + s_pickMargin) && distx > (1.0f - s_pickMargin))
		{
			bestDist = abs(distx-1.0f);
			axis = RotationAxis::AXIS_X;
		}		

		if(disty < (1.0f + s_pickMargin) && disty > (1.0f - s_pickMargin) && abs(disty-1.0f) < bestDist)
		{
			bestDist = abs(disty-1.0f);
			axis = RotationAxis::AXIS_Y;
		}	

		if(distz < (1.0f + s_pickMargin) && distz > (1.0f - s_pickMargin) && abs(distz-1.0f) < bestDist)
		{
			bestDist = abs(distz-1.0f);
			axis = RotationAxis::AXIS_Z;
		}	

		// Set selected axis to hoovered axis and store the current rotation of gimbal.
		m_angle0 = GetAngleFromAxis(axis, hits[axis] - gimbalTransform->m_position);
		m_selectedAxis = axis;
		return true;
	}
	else if(leftMouseButtonState == RootEngine::InputManager::KeyState::UP_EDGE && m_selectedAxis != RotationAxis::AXIS_NONE)
	{
		// Reset.
		m_angle0 = 0.0f;
		m_selectedAxis = RotationAxis::AXIS_NONE;
		return true;
	}
	else if(leftMouseButtonState == RootEngine::InputManager::KeyState::DOWN && m_selectedAxis != RotationAxis::AXIS_NONE)
	{
		m_axisColors[m_selectedAxis] = s_hightlightColor;

		g_engineContext.m_renderer->AddLine(gimbalTransform->m_position, hits[m_selectedAxis], glm::vec4(1,1,0,1));

		float angle = GetAngleFromAxis(m_selectedAxis, hits[m_selectedAxis] - gimbalTransform->m_position);

		switch(m_selectedAxis)
		{
			case RotationAxis::AXIS_X:
				{
					for(int i = 0; i < 3; i++)
					{
						m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_axisEntities[i])->m_orientation = m_gimbalOrientation;
						m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_axisEntities[i])->m_orientation.Pitch(glm::degrees(m_angle0 - angle));

						selectedTransform->m_orientation = m_entityOrientation;
						selectedTransform->m_orientation.Pitch(glm::degrees(m_angle0 - angle));
					}

					std::cout << "X plane " << glm::degrees(angle) << " Angle0 " << glm::degrees(m_angle0) << std::endl << std::endl;
				}
				break;
			case RotationAxis::AXIS_Y:
				{
					for(int i = 0; i < 3; i++)
					{
						m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_axisEntities[i])->m_orientation = m_gimbalOrientation;
						m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_axisEntities[i])->m_orientation.Yaw(glm::degrees(m_angle0 - angle));

						selectedTransform->m_orientation = m_entityOrientation;
						selectedTransform->m_orientation.Yaw(glm::degrees(m_angle0 - angle));
					}

					std::cout << "Y plane " << glm::degrees(angle) << " Angle0 " << glm::degrees(m_angle0) << std::endl << std::endl;
				}
				break;
			case RotationAxis::AXIS_Z:
				{
					for(int i = 0; i < 3; i++)
					{
						m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_axisEntities[i])->m_orientation = m_gimbalOrientation;
						m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_axisEntities[i])->m_orientation.Roll(-glm::degrees(m_angle0 - angle));

						selectedTransform->m_orientation = m_entityOrientation;
						selectedTransform->m_orientation.Roll(-glm::degrees(m_angle0 - angle));
					}

					std::cout << "Z plane " << glm::degrees(angle) << " Angle0 " << glm::degrees(m_angle0) << std::endl << std::endl;
				}
				break;
			default:
				break;
		}
		return true;
	}

	return false;
}

void RotationTool::SetPosition(const glm::vec3& p_position)
{
	m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_axisEntities[RotationAxis::AXIS_X])->m_position = p_position;
	m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_axisEntities[RotationAxis::AXIS_Y])->m_position = p_position;
	m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_axisEntities[RotationAxis::AXIS_Z])->m_position = p_position;
}

void RotationTool::Hide()
{
	m_world->GetEntityManager()->RemoveComponent<RootForce::Transform>(m_axisEntities[RotationAxis::AXIS_X]);
	m_world->GetEntityManager()->RemoveComponent<RootForce::Transform>(m_axisEntities[RotationAxis::AXIS_Y]);
	m_world->GetEntityManager()->RemoveComponent<RootForce::Transform>(m_axisEntities[RotationAxis::AXIS_Z]);
}

void RotationTool::Show()
{
	m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(m_axisEntities[RotationAxis::AXIS_X]);
	m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(m_axisEntities[RotationAxis::AXIS_Y]);
	m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(m_axisEntities[RotationAxis::AXIS_Z]);

	m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_axisEntities[RotationAxis::AXIS_X])->m_orientation = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_selectedEntity)->m_orientation;
	m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_axisEntities[RotationAxis::AXIS_Y])->m_orientation = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_selectedEntity)->m_orientation;
	m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_axisEntities[RotationAxis::AXIS_Z])->m_orientation = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_selectedEntity)->m_orientation;
}
