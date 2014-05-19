#include <RootTools/Treenity/Include/Tool.h>

void Tool::SetSelectedEntity(ECS::Entity* p_entity)
{
	if(p_entity == nullptr)
	{
		Hide();
		m_selectedEntity = nullptr;
	}
	else 
	{
		m_selectedEntity = p_entity;
		Show();
	}
}

ECS::Entity* Tool::GetSelectedEntity()
{
	return m_selectedEntity;
}