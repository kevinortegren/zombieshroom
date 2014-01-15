#include "OnCreate.h"

namespace AbilityEditorNameSpace
{


	OnCreate::OnCreate()
	{

	}

	OnCreate::~OnCreate()
	{

	}

	void OnCreate::AddEntity( std::string p_name )
	{
		AbilityEntity::Entity* entity = new AbilityEntity::Entity(p_name);
		m_entities.push_back(entity);
	}

	void OnCreate::AddEntity( AbilityEntity::Entity* p_entity )
	{
		m_entities.push_back(p_entity);
	}

	void OnCreate::AddComponent(unsigned int p_id, AbilityComponents::ComponentType::ComponentType p_type )
	{
		m_entities.at(p_id)->AddComponent(p_type);
	}

}