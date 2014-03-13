#pragma once

#ifndef COMPILE_LEVEL_EDITOR

#include <string>
#include <map>
#include <Utility/ECS/Include/Component.h>
#include <RootSystems/Include/Network/NetworkTypes.h>

namespace RootForce
{
    struct Script : public ECS::Component<Script>
	{
		static const size_t MAX_NUMBER_STORAGE = 32;
		static const size_t MAX_STRING_STORAGE = 32;
		static const size_t MAX_ENTITY_STORAGE = 32;
		static const size_t MAX_STRING_LENGTH = 1024;

		std::string Name;
		std::map<std::string, float> StorageNumber;
		std::map<std::string, std::string> StorageString;
		std::map<std::string, Network::NetworkEntityID> StorageEntity;

		void InsertNumber(const std::string& p_key, float p_value);
		void InsertString(const std::string& p_key, const std::string& p_value);
		void InsertEntity(const std::string& p_key, ECS::Entity* p_value);
		void InsertEntity(const std::string& p_key, const Network::NetworkEntityID& p_id);
		float GetNumber(const std::string& p_key) const;
		std::string GetString(const std::string& p_key) const;
		ECS::Entity* GetEntity(const std::string& p_key) const;
	};
}

#endif