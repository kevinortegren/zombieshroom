#pragma once

#include "AbilityComponents.h"
namespace AbilityEditor
{
	class OnDestroy
	{
	public:
		OnDestroy();
		~OnDestroy();
		void AddEntity(std::string p_name);
		void AddComponent(AbilityComponents::ComponentType::ComponentType p_type);
		std::vector<AbilityEntity::Entity>* GetEntities() { return &m_entities; }
	private:
		std::vector<AbilityEntity::Entity> m_entities;
	};
}