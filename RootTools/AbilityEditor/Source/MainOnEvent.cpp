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
	void MainOnEvent::AddEntity( QString p_name )
	{
		AbilityEntity::Entity* entity = new AbilityEntity::Entity(p_name);
		m_entities.push_back(entity);
	}

	void MainOnEvent::AddEntity( AbilityEntity::Entity* p_entity )
	{
		m_entities.push_back(p_entity);
	}

	void MainOnEvent::AddComponent(unsigned int p_id, AbilityComponents::ComponentType::ComponentType p_type )
	{
		m_entities.at(p_id)->AddComponent(p_type);
	}

	void MainOnEvent::AddComponent( unsigned int p_id, QString p_name )
	{
		m_entities.at(p_id)->AddComponent(p_name);
	}

	void MainOnEvent::RemoveEntity( unsigned int p_id )
	{
		delete m_entities.at(p_id);
		m_entities.erase(m_entities.begin() + p_id);
	}

	void MainOnEvent::RemoveComponent( unsigned int p_id, AbilityComponents::ComponentType::ComponentType p_type )
	{
		m_entities.at(p_id)->RemoveComponent(p_type);
	}

	void MainOnEvent::RemoveComponent( unsigned int p_id, QString p_name )
	{
		m_entities.at(p_id)->RemoveComponent(p_name);
	}


	bool MainOnEvent::CheckForExistingComponents( unsigned int p_id, AbilityComponents::ComponentType::ComponentType p_type )
	{
		return m_entities.at(p_id)->DoesComponentExist(p_type);
	}

	bool MainOnEvent::CheckForExistingComponents( unsigned int p_id, QString p_name )
	{
		return m_entities.at(p_id)->DoesComponentExist(p_name);
	}

	void MainOnEvent::ViewEntityData( unsigned int p_id, QtTreePropertyBrowser* p_propBrows )
	{

		QString name = m_entities.at(p_id)->m_name;
		QtVariantProperty* prop;
		prop = m_propManager->addProperty(QVariant::String, "Name" );
		m_propManager->setValue(prop, name);
		p_propBrows->setFactoryForManager(m_propManager, m_propFactory);
		p_propBrows->addProperty(prop);

		
	}

	void MainOnEvent::ViewComponentData( unsigned int p_id, QtTreePropertyBrowser* p_propBrows, QString p_name )
	{
		m_entities.at(p_id)->ViewComponentData(m_propManager,p_propBrows, m_propFactory , p_name);

	}

	void MainOnEvent::EditEntityData( unsigned int p_id, QtTreePropertyBrowser* p_propBrows )
	{
		QList<QtProperty*> props;
		props = p_propBrows->properties();
		m_entities.at(p_id)->m_name = props.at(0)->valueText();
	}

	void MainOnEvent::EditComponentData( unsigned int p_id, QtTreePropertyBrowser* p_propBrows, QString p_name )
	{
		m_entities.at(p_id)->SaveComponentData(m_propManager,p_propBrows, m_propFactory , p_name);
	}

}