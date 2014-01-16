#include "OnCollide.h"

namespace AbilityEditorNameSpace
{


	OnCollide::OnCollide()
	{

	}

	OnCollide::~OnCollide()
	{

	}

	void OnCollide::AddEntity( std::string p_name )
	{
		AbilityEntity::Entity* entity = new AbilityEntity::Entity(p_name);
		m_entities.push_back(entity);
	}

	void OnCollide::AddEntity( AbilityEntity::Entity* p_entity )
	{
		m_entities.push_back(p_entity);
	}

	void OnCollide::AddComponent(unsigned int p_id, AbilityComponents::ComponentType::ComponentType p_type )
	{
		m_entities.at(p_id)->AddComponent(p_type);
	}

	void OnCollide::AddComponent( unsigned int p_id, QString p_name )
	{
		m_entities.at(p_id)->AddComponent(p_name);
	}

	void OnCollide::RemoveComponent( unsigned int p_id, AbilityComponents::ComponentType::ComponentType p_type )
	{
		m_entities.at(p_id)->RemoveComponent(p_type);
	}

	void OnCollide::RemoveComponent( unsigned int p_id, QString p_name )
	{
		m_entities.at(p_id)->RemoveComponent(p_name);
	}

	void OnCollide::ManipulateData( QtTreePropertyBrowser* p_propBrows )
	{

	}

}