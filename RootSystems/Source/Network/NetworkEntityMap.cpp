#include <Network/NetworkEntityMap.h>
#include <Utility/ECS/Include/World.h>

namespace RootForce
{
	namespace Network
	{
		NetworkEntityMap::NetworkEntityMap()
			: m_nextSynchronizedId(0)
			, m_nextTemporaryId(0)
		{}


		TemporaryId_t NetworkEntityMap::AddEntity(ECS::Entity* p_entity)
		{
			m_temporaryEntityMap[m_nextTemporaryId] = p_entity;
			return m_nextTemporaryId++;
		}

		void NetworkEntityMap::SetSynchronizedId(TemporaryId_t p_temporaryId, SynchronizedId_t p_synchronizedId)
		{
			m_synchronizedEntityMap[p_synchronizedId] = m_temporaryEntityMap[p_temporaryId];
			m_temporaryEntityMap.erase(m_temporaryEntityMap.find(p_temporaryId));
		}

		void NetworkEntityMap::RemoveSynchronizedEntity(SynchronizedId_t p_synchronizedId)
		{
			m_synchronizedEntityMap.erase(p_synchronizedId);
		}

		SynchronizedId_t NetworkEntityMap::GetSynchronizedId(ECS::Entity* p_entity) const
		{
			for (auto it = m_synchronizedEntityMap.begin(); it != m_synchronizedEntityMap.end(); it++)
			{
				if (it->second == p_entity)
					return it->first;
			}

			return SYNCHRONIZED_ID_NONE;
		}

		TemporaryId_t NetworkEntityMap::GetTemporaryId(ECS::Entity* p_entity) const
		{
			for (auto it = m_temporaryEntityMap.begin(); it != m_temporaryEntityMap.end(); it++)
			{
				if (it->second == p_entity)
					return it->first;
			}

			return TEMPORARY_ID_NONE;
		}

		ECS::Entity* NetworkEntityMap::GetSynchronizedEntity(SynchronizedId_t p_synchronizedId) const
		{
			auto it = m_synchronizedEntityMap.find(p_synchronizedId);
			if (it == m_synchronizedEntityMap.end())
				return nullptr;
			else
				return it->second;
		}

		SynchronizedId_t NetworkEntityMap::NextSynchronizedId()
		{
			return m_nextSynchronizedId++;
		}
	}
}