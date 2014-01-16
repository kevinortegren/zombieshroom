#include "OnDestroy.h"
namespace AbilityEditorNameSpace
{

	OnDestroy::OnDestroy()
	{

	}

	OnDestroy::~OnDestroy()
	{

	}

	void OnDestroy::AddEntity( std::string p_name )
	{
		AbilityEntity::Entity* entity = new AbilityEntity::Entity(p_name);
		m_entities.push_back(entity);
	}

	void OnDestroy::AddEntity( AbilityEntity::Entity* p_entity )
	{
		m_entities.push_back(p_entity);
	}

	void OnDestroy::AddComponent(unsigned int p_id, AbilityComponents::ComponentType::ComponentType p_type )
	{
		m_entities.at(p_id)->AddComponent(p_type);
	}

	void OnDestroy::AddComponent( unsigned int p_id, QString p_name )
	{
		m_entities.at(p_id)->AddComponent(p_name);
	}

	void OnDestroy::RemoveComponent( unsigned int p_id, AbilityComponents::ComponentType::ComponentType p_type )
	{
		m_entities.at(p_id)->RemoveComponent(p_type);
	}

	void OnDestroy::RemoveComponent( unsigned int p_id, QString p_name )
	{
		m_entities.at(p_id)->RemoveComponent(p_name);
	}

	void OnDestroy::ManipulateData( QtTreePropertyBrowser* p_propBrows )
	{

	}

	
}

