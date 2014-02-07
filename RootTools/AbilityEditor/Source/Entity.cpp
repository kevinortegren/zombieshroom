#include "Entity.h"

namespace AbilityEditorNameSpace
{
	Entity::Entity(QString p_name)
	{
		for(int i = 0 ; i < AbilityComponents::ComponentType::END_OF_ENUM; i++)
		{
			m_nameToEnumMapper[AbilityComponents::g_componentNameList.m_compNames.at(i)] = (AbilityComponents::ComponentType::ComponentType)i;
		}
		m_name = p_name;
		m_components = new std::vector<AbilityComponents::MainComponent*>();
	}
	Entity::~Entity()
	{
		if(m_components->size() > 0)
		{
			for(unsigned int i = m_components->size() -1 ; i > 0; i --)
			{
				delete m_components->at(i);
				m_components->pop_back();
			}
		}
		delete m_components;
	}
	void Entity::AddComponent(QString p_componentName)
	{
		AddComponent((AbilityComponents::ComponentType::ComponentType)m_nameToEnumMapper.at(p_componentName));
	}
	void Entity::AddComponent(AbilityComponents::ComponentType::ComponentType p_type)
	{
		switch (p_type)
		{

		case AbilityComponents::ComponentType::TRANSFORM:
			{
				AbilityComponents::MainComponent* temp = new AbilityComponents::Transform();
				AddComponent(temp);
			}
			break;
		case AbilityComponents::ComponentType::COLLISION:			
			{
				AbilityComponents::MainComponent* temp = new AbilityComponents::Collision();
				AddComponent(temp);
			}
			break;
		case AbilityComponents::ComponentType::ABILITYMODEL:
			{
				AbilityComponents::MainComponent* temp = new AbilityComponents::AbilityModel();
				AddComponent(temp);
			}
			break;
		case AbilityComponents::ComponentType::COLLISIONSHAPE:
			{
				AbilityComponents::MainComponent* temp = new AbilityComponents::CollisionShape();
				AddComponent(temp);
			}
			break;
		case AbilityComponents::ComponentType::PHYSICSCONTROLLED:
			{
				AbilityComponents::MainComponent* temp = new AbilityComponents::PhysicsControlled();
				AddComponent(temp);
			}
			break;
		case AbilityComponents::ComponentType::ABILITYPARTICLE:
			{
				AbilityComponents::MainComponent* temp = new AbilityComponents::AbilityParticle();
				AddComponent(temp);
			}
			break;
		case AbilityComponents::ComponentType::OFFENSIVEABILITY:
			{
				AbilityComponents::MainComponent* temp = new AbilityComponents::OffensiveAbility();
				AddComponent(temp);
			}
			break;
		case AbilityComponents::ComponentType::EXPLOSIVE:
			{
				AbilityComponents::MainComponent* temp = new AbilityComponents::Explosive();
				AddComponent(temp);
			}
			break;
		default:
			break; 
		}
	}
	void Entity::AddComponent(AbilityComponents::MainComponent* p_component)
	{
		m_components->push_back(p_component);
	}
	void Entity::RemoveComponent(QString p_componentName)
	{
		RemoveComponent((AbilityComponents::ComponentType::ComponentType)m_nameToEnumMapper.at(p_componentName));
	}
	void Entity::RemoveComponent(AbilityComponents::ComponentType::ComponentType p_type)
	{
		for(unsigned int i = 0; i < m_components->size(); i++)
			if(m_components->at(i)->m_type == p_type)
			{
				delete m_components->at(i);
				m_components->erase(m_components->begin() + i);
				break;
			}
	}
	bool Entity::DoesComponentExist(QString p_componentName)
	{
		return DoesComponentExist((AbilityComponents::ComponentType::ComponentType)m_nameToEnumMapper.at(p_componentName));
	}
	bool Entity::DoesComponentExist(AbilityComponents::ComponentType::ComponentType p_type)
	{
		for(unsigned int i = 0; i < m_components->size(); i++)
		{
			if(m_components->at(i)->m_type == p_type)
				return true;
		}
		return false;
	}
	void Entity::ViewComponentData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory ,  QString p_name)
	{
		AbilityComponents::ComponentType::ComponentType type = (AbilityComponents::ComponentType::ComponentType)m_nameToEnumMapper[p_name];
		for(unsigned int i = 0 ; i < m_components->size(); i++)
		{
			if(m_components->at(i)->m_type == type)
			{
				m_components->at(i)->ViewData(p_propMan, p_propBrows, p_factory);
			}
		}
	}
	void Entity::SaveComponentData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory ,  QString p_name)
	{
		AbilityComponents::ComponentType::ComponentType type = (AbilityComponents::ComponentType::ComponentType)m_nameToEnumMapper[p_name];
		for(unsigned int i = 0 ; i < m_components->size(); i++)
		{
			if(m_components->at(i)->m_type == type)
			{
				m_components->at(i)->SaveData(p_propMan,p_propBrows, p_factory);
			}
		}
	}
	QString Entity::GetComponentNameFromId(unsigned int p_id)
	{
		AbilityComponents::ComponentType::ComponentType findType = m_components->at(p_id)->m_type;
		std::map<QString, AbilityComponents::ComponentType::ComponentType>::const_iterator it;
		for(it = m_nameToEnumMapper.begin(); it != m_nameToEnumMapper.end(); ++it)
		{
			if(it->second == findType)
			{
				return it->first;
			}
		}
		return "";
	}
	void Entity::RemoveAllComponents()
	{
		for(unsigned int i = 0; i < m_components->size(); i++)
		{	
			delete m_components->at(i);
			m_components->erase(m_components->begin() + i);
		}
	}
}