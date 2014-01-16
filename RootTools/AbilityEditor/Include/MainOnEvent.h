#pragma once
#include "AbilityComponents.h"
#include <QtPropertyBrowser/QtTreePropertyBrowser>
namespace AbilityEditorNameSpace
{
	class  MainOnEvent
	{
	public:
		MainOnEvent();
		~MainOnEvent();
		virtual void AddEntity(std::string p_name) = 0;
		virtual void AddEntity(AbilityEntity::Entity* p_entity) = 0;
		virtual void AddComponent(unsigned int p_id, AbilityComponents::ComponentType::ComponentType p_type) = 0;
		virtual void AddComponent(unsigned int p_id, QString p_name) = 0;
		virtual void RemoveComponent(unsigned int p_id, AbilityComponents::ComponentType::ComponentType p_type) = 0;
		virtual void RemoveComponent(unsigned int p_id, QString p_name) = 0;
#pragma warning (Här är du)
		virtual void ManipulateData(QtTreePropertyBrowser* p_propBrows) = 0;
	protected:
		QtTreePropertyBrowser* m_propBrows;
		
	};
}