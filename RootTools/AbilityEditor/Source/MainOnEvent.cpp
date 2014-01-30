#include "MainOnEvent.h"

namespace AbilityEditorNameSpace
{


	MainOnEvent::MainOnEvent()
	{
		m_propManager = new QtVariantPropertyManager;
		m_propFactory = new QtVariantEditorFactory;
	}

	MainOnEvent::~MainOnEvent()
	{

	}
	void MainOnEvent::AddCondition( QString p_name )
	{
		// TODO: Add condition by name maybe?
		//AbilityEntity::Entity* entity = new AbilityEntity::Entity(p_name);
		//m_entities.push_back(entity);
	}

	void MainOnEvent::AddCondition( Condition* p_cond )
	{
		m_conditions.push_back(p_cond);
	}

	void MainOnEvent::AddEntity( unsigned int p_id, QString p_name )
	{
		m_conditions.at(p_id)->AddEntity(p_name);
	}

	void MainOnEvent::RemoveEntity( unsigned int p_id, QString p_name )
	{
		m_conditions.at(p_id)->RemoveEntity(p_name);
	}

	void MainOnEvent::RemoveCondition( unsigned int p_id )
	{
		delete m_conditions.at(p_id);
		m_conditions.erase(m_conditions.begin() + p_id);
		
	}

	void MainOnEvent::ViewConditionData( unsigned int p_id, QtTreePropertyBrowser* p_propBrows, QtVariantPropertyManager* p_propMan )
	{
		////m_propManager = p_propMan;
		//QString name = m_entities.at(p_id)->m_name;
		//QtVariantProperty* prop;
		//prop = p_propMan->addProperty(QVariant::String, "Name" );
		//p_propMan->setValue(prop, name);
		//p_propBrows->setFactoryForManager(p_propMan, m_propFactory);
		//p_propBrows->addProperty(prop);

		//TODO: Do stuff here
	}

	void MainOnEvent::EditConditionData( unsigned int p_id, QtTreePropertyBrowser* p_propBrows, QtVariantPropertyManager* p_propMan )
	{
		////m_propManager = p_propMan;
		//QList<QtProperty*> props;
		//props = p_propBrows->properties();
		//m_entities.at(p_id)->m_name = props.at(0)->valueText();

		//Same as above
	}


	void MainOnEvent::Clear()
	{
		for(unsigned int i = 0; i < m_conditions.size(); i++)
		{
			//m_entities.at(i)->RemoveAllComponents();
			delete m_conditions.at(i);
		}	
		m_conditions.clear();
		//m_scripts->clear(); //Scriptlist
	}

	std::vector<QString> MainOnEvent::GetEntityNames(unsigned int p_id)
	{
		return m_conditions.at(p_id)->GetEntities();
	}

}