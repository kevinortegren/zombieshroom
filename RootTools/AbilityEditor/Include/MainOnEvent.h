#pragma once
#include "AbilityComponents.h"

namespace AbilityEditorNameSpace
{
	class  MainOnEvent
	{
	public:
		MainOnEvent();
		~MainOnEvent();
		virtual void AddScript(QString p_name);
		virtual void AddEntity(QString p_name);
		virtual void AddEntity(AbilityEntity::Entity* p_entity);
		virtual void AddComponent(unsigned int p_id, AbilityComponents::ComponentType::ComponentType p_type);
		virtual void AddComponent(unsigned int p_id, QString p_name);
		// virtual void RemoveScript();
		virtual void RemoveEntity(unsigned int p_id);
		virtual void RemoveComponent(unsigned int p_id, AbilityComponents::ComponentType::ComponentType p_type);
		virtual void RemoveComponent(unsigned int p_id, QString p_name);
		virtual bool CheckForExistingComponents(unsigned int p_id,  AbilityComponents::ComponentType::ComponentType p_type);
		virtual bool CheckForExistingComponents(unsigned int p_id, QString p_name);

		virtual QString GetEntityName(unsigned int p_id) const{return m_entities.at(p_id)->m_name;}

		virtual void ViewEntityData(unsigned int p_id, QtTreePropertyBrowser* p_propBrows, QtVariantPropertyManager* p_propMan);
		virtual void ViewComponentData(unsigned int p_id, QtTreePropertyBrowser* p_propBrows, QString p_name, QtVariantPropertyManager* p_propMan);
		virtual void EditEntityData(unsigned int p_id, QtTreePropertyBrowser* p_propBrows, QtVariantPropertyManager* p_propMan);
		virtual void EditComponentData(unsigned int p_id, QtTreePropertyBrowser* p_propBrows, QString p_name, QtVariantPropertyManager* p_propMan);
		virtual void Clear();
		//virtual QtVariantPropertyManager* PointMeToPropMan() { return m_propManager; }
		
		virtual std::vector<QString> GetScriptNames() { return m_scripts; }
		virtual std::vector<AbilityEntity::Entity*>* GetEntities() { return &m_entities; }
	protected:
		//QtTreePropertyBrowser* m_propBrows;
		std::vector<AbilityEntity::Entity*> m_entities;
		QtVariantPropertyManager* m_propManager;
		QtVariantEditorFactory* m_propFactory;
		std::vector<QString> m_scripts;
	};
}