#pragma once

#include "AbilityComponents.h"

namespace AbilityEditorNameSpace
{

	class Entity
	{
	public:
		Entity::Entity(QString p_name);
		Entity::~Entity();

		void AddComponent(QString p_componentName);
		void AddComponent(AbilityComponents::ComponentType::ComponentType p_type);
		void AddComponent(AbilityComponents::MainComponent* p_component);
		void RemoveComponent(QString p_componentName);
		void RemoveComponent(AbilityComponents::ComponentType::ComponentType p_type);
		bool DoesComponentExist(QString p_componentName);
		bool DoesComponentExist(AbilityComponents::ComponentType::ComponentType p_type);
		void ViewComponentData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory , QString p_name);
		void SaveComponentData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory , QString p_name);
		QString GetComponentNameFromId(unsigned int p_id);
		
		void RemoveAllComponents();
		void SetName(QString p_name) { m_name = p_name; }
		void SetCooldown(float p_cooldown) { m_cooldown = p_cooldown; }
		QString GetName() { return m_name; }
		float GetCooldown() { return m_cooldown; }
		std::vector<AbilityComponents::MainComponent*>* GetComponents() { return m_components; }
	private:
		QString m_name;
		float m_cooldown;
		std::vector<AbilityComponents::MainComponent*>* m_components;
		std::map<QString, AbilityComponents::ComponentType::ComponentType> m_nameToEnumMapper;
		
		
	};
}