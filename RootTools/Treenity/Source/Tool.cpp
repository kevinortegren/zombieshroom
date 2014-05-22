#include <RootTools/Treenity/Include/Tool.h>
#include <RootSystems/Include/Transform.h>

Tool::Tool()
	: m_selectedEntity(nullptr),
	m_visible(false)
{

}

void Tool::UpdateRenderingPosition()
{
	if(m_selectedEntity == nullptr)
		return;

	ECS::Entity* cameraEntity = m_world->GetTagManager()->GetEntityByTag("Camera"); 
	glm::vec3 cameraPos = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(cameraEntity)->m_position;

	RootForce::Transform* cameraTransform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(cameraEntity);
	RootForce::Transform* selectedTransform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_selectedEntity);

	glm::vec3 dir = -glm::normalize(cameraPos - selectedTransform->m_position);
	float t = 8.0f / glm::dot(cameraTransform->m_orientation.GetFront(), dir);

	SetPosition(cameraPos + t * dir);
}

void Tool::SetSelectedEntity(ECS::Entity* p_entity)
{
	if(p_entity == nullptr)
	{
		if(m_selectedEntity != nullptr)
		{
			Hide();
			m_selectedEntity = nullptr;
		}
	}
	else 
	{
		if(m_selectedEntity != p_entity)
		{
			m_selectedEntity = p_entity;
			Show();			
		}
	}
}

ECS::Entity* Tool::GetSelectedEntity()
{
	return m_selectedEntity;
}

bool Tool::IsVisible()
{
	return m_visible;
}

void Tool::Hide()
{
	m_visible = false;
}

void Tool::Show()
{
	m_visible = true;
}
