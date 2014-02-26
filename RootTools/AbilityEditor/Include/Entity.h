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
		QString GetName() { return m_name; }

		void SetCooldown(float p_cooldown) { m_cooldown = p_cooldown; }
		float GetCooldown() { return m_cooldown; }

		void SetCharges(int p_charges) { m_charges = p_charges; }
		int GetCharges() { return m_charges; }

		void SetChargeTime(float p_chargeTime) { m_chargeTime = p_chargeTime; }
		float GetChargeTime() { return m_chargeTime; }

		void SetChannelingTime(float p_channelingTime) { m_channelingTime = p_channelingTime; }
		float GetChannelingTime() { return m_channelingTime; }

		void SetDuration(float p_duration) { m_duration = p_duration; }
		float GetDuration() { return m_duration; }

		std::vector<AbilityComponents::MainComponent*>* GetComponents() { return m_components; }
	private:
		QString m_name;
		float m_cooldown;
		int m_charges;
		float m_chargeTime;
		float m_channelingTime;
		float m_duration;
		std::vector<AbilityComponents::MainComponent*>* m_components;
		std::map<QString, AbilityComponents::ComponentType::ComponentType> m_nameToEnumMapper;
		
		
	};
}