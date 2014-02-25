#ifndef COMPILE_LEVEL_EDITOR

#include <RootSystems/Include/Network/NetworkTypes.h>
#include <Utility/ECS/Include/Entity.h>
#include <Utility/ECS/Include/EntityManager.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/Script.h>
#include <RootEngine/Script/Include/RootScript.h>
#include <vector>

extern RootEngine::GameSharedContext g_engineContext;

namespace RootForce
{
	namespace Network
	{
		void DeleteEntities(NetworkEntityMap& p_map, NetworkEntityID p_id, ECS::EntityManager* p_entityManager)
		{
			std::vector<NetworkEntityMap::iterator> destroy;
			for (auto it = p_map.begin(); it != p_map.end(); it++)
			{
				// Check if the given ID matches the user
				if (p_id.UserID == ReservedUserID::ALL || it->first.UserID == p_id.UserID)
				{
					// Check if the given ID matches the action
					if (p_id.ActionID == ReservedActionID::ALL || it->first.ActionID == p_id.ActionID)
					{
						// Check if the given ID matches the sequence
						if (p_id.SequenceID == ReservedSequenceID::ALL || it->first.SequenceID == p_id.SequenceID)
						{
							destroy.push_back(it);
						}
					}
				}
			}

			for (size_t i = 0; i < destroy.size(); ++i)
			{
				// If the entity has a script component, call its OnDestroy script.
				if(!destroy[i]->second)
					continue;
				Script* script = p_entityManager->GetComponent<Script>(destroy[i]->second);
				if (script != nullptr)
				{
					g_engineContext.m_script->SetFunction(script->Name, "OnDestroy");
					g_engineContext.m_script->AddParameterUserData(destroy[i]->second, sizeof(ECS::Entity*), "Entity");
					g_engineContext.m_script->ExecuteScript();
				}

				// Remove the entity from the world
				//p_entityManager->RemoveAllComponents(destroy[i]->second);
				//p_entityManager->RemoveEntity(destroy[i]->second);
				//p_map.erase(destroy[i]);
			}
		}


		ECS::Entity* FindEntity(NetworkEntityMap& p_map, const NetworkEntityID& p_id)
		{
			NetworkEntityMap::iterator it = p_map.find(p_id);
			if (it == p_map.end())
				return nullptr;
			return it->second;
		}
	}
}

#endif