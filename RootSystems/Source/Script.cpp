#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootSystems/Include/Script.h>
#include <RootSystems/Include/Network/NetworkComponents.h>

extern RootEngine::GameSharedContext g_engineContext;
extern RootForce::Network::NetworkEntityMap g_networkEntityMap;
extern ECS::World* g_world;

namespace RootForce
{
	void Script::InsertNumber(const std::string& p_key, float p_value)
	{
		auto it = StorageNumber.find(p_key);
		if (it != StorageNumber.end() || StorageNumber.size() < MAX_NUMBER_STORAGE)
		{
			StorageNumber[p_key] = p_value;
		}
	}

	void Script::InsertString(const std::string& p_key, const std::string& p_value)
	{
		auto it = StorageString.find(p_key);
		if (it != StorageString.end() || StorageString.size() < MAX_NUMBER_STORAGE)
		{
			StorageString[p_key] = p_value;
		}
	}

	void Script::InsertEntity(const std::string& p_key, ECS::Entity* p_value)
	{
		Network::NetworkComponent* network = g_world->GetEntityManager()->GetComponent<Network::NetworkComponent>(p_value);
		if (network != nullptr)
		{
			InsertEntity(p_key, network->ID);
		}
		else
		{
			g_engineContext.m_logger->LogText(LogTag::SCRIPT, LogLevel::NON_FATAL_ERROR, "Attemped to store entity without network component.");
		}
	}

	void Script::InsertEntity(const std::string& p_key, const Network::NetworkEntityID& p_id)
	{
		auto it = StorageEntity.find(p_key);
		if (it != StorageEntity.end() || StorageEntity.size() < MAX_NUMBER_STORAGE)
		{
			StorageEntity[p_key] = p_id;
		}
	}

	float Script::GetNumber(const std::string& p_key) const
	{
		auto it = StorageNumber.find(p_key);
		if (it != StorageNumber.end())
		{
			return it->second;
		}

		return 0.0f;
	}

	std::string Script::GetString(const std::string& p_key) const
	{
		auto it = StorageString.find(p_key);
		if (it != StorageString.end())
		{
			return it->second;
		}

		return "";
	}

	ECS::Entity* Script::GetEntity(const std::string& p_key) const
	{
		auto it = StorageEntity.find(p_key);
		if (it != StorageEntity.end())
		{
			return Network::FindEntity(g_networkEntityMap, it->second);
		}

		return nullptr;
	}

}