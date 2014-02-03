#pragma once
#include "AbilityComponents.h"
#include "Condition.h"

namespace AbilityEditorNameSpace
{
	class  MainOnEvent
	{
	public:
		MainOnEvent();
		~MainOnEvent();
		virtual void AddEntity(unsigned int p_id, QString p_name); 
		virtual void AddCondition(QString p_name, QString p_code); //Needed?
		virtual void AddCondition(Condition* p_cond);

		virtual void RemoveEntity(unsigned int p_id, QString p_name);
		virtual void RemoveCondition( unsigned int p_id );

		virtual void ViewConditionData(unsigned int p_id, QtTreePropertyBrowser* p_propBrows, QtVariantPropertyManager* p_propMan);
		virtual void EditConditionData(unsigned int p_id, QtTreePropertyBrowser* p_propBrows, QtVariantPropertyManager* p_propMan);
		virtual void Clear();
		//virtual QtVariantPropertyManager* PointMeToPropMan() { return m_propManager; }
		
		virtual std::vector<QString> GetEntityNames(unsigned int p_id);
		virtual std::vector<Condition*>* GetConditions() { return &m_conditions; }
	protected:

		std::vector<Condition*> m_conditions;
		QtVariantPropertyManager* m_propManager;
		QtVariantEditorFactory* m_propFactory;
	};
}