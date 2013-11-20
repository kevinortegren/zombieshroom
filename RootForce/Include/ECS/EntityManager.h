#pragma once

#include <ECS/Entity.h>
#include <ECS/ComponentInterface.h>
#include <memory>
#include <vector>
#include <stack>

namespace ECS
{
	class EntityManager
	{
	public:
		EntityManager();

		std::shared_ptr<Entity> CreateEntity();
		void RemoveEntity(std::shared_ptr<Entity> p_entity);

		template<class C> 
		std::shared_ptr<C> CreateComponent(int p_componentType, std::shared_ptr<Entity> entity)
		{
			std::shared_ptr<C> component = std::shared_ptr<C>(new C);



		}

	private:
		int m_nextID;
		std::vector<std::shared_ptr<Entity>> m_entities;
		std::stack<int> m_recyledIds;
		std::vector<std::vector<ComponentInterface>> m_components;

	};
}