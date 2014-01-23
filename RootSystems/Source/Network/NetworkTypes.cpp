#include <RootSystems/Include/Network/NetworkTypes.h>
#include <Utility/ECS/Include/Entity.h>
#include <Utility/ECS/Include/EntityManager.h>
#include <vector>

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
				p_entityManager->RemoveAllComponents(destroy[i]->second);
				p_entityManager->RemoveEntity(destroy[i]->second);
				p_map.erase(destroy[i]);
			}
		}
	}
}