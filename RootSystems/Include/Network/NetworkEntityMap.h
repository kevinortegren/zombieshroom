#pragma once

#include <stdint.h>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/RakString.h>
#include <RootSystems/Include/Components.h>
#include <RootSystems/Include/Network/NetworkTypes.h>
#include <Utility/ECS/Include/Entity.h>
#include <Utility/ECS/Include/Component.h>

namespace RootForce
{
	namespace Network
	{
		/*
			Used to associate a NetworkEntityID with a local entity.
		*/
		typedef std::map<NetworkEntityID, ECS::Entity*> NetworkEntityMap;
		

		void SerializeEntity(RakNet::BitStream* p_bs, ECS::Entity* p_entity, const std::string& p_entityScript, const NetworkEntityMap& p_map, ECS::World* p_world);
		ECS::Entity* DeserializeEntity(RakNet::BitStream* p_bs, const NetworkEntityMap& p_map, ECS::World* p_world);


		/*
			Defines a serialized component that can be sent across the network
		*/
		/*
		struct SerializedComponent
		{
			ComponentType::ComponentType Type;
			unsigned int DataSize;
			unsigned char* Data;

			void Serialize(bool p_write, RakNet::BitStream* p_bs);
			void SerializeComponent(ECS::ComponentInterface* p_component, ComponentType::ComponentType p_type);
			void DeserializeComponent(ECS::ComponentInterface* p_component);
		};
		*/

		/*
			Defines a serialized entity that can be sent across the network
		*/
		/*
		struct SerializedEntity
		{
			NetworkEntityID ID;
			RakNet::RakString Script;
			std::vector<SerializedComponent> Components;

			void Serialize(bool p_writeToBitstream, RakNet::BitStream* p_bs);
			void SerializeEntity(ECS::Entity* p_entity);
			void DeserializeEntity(ECS::Entity* p_entity);
		};
		*/

		/*
			Sent occasionally to clients to synchronize entities.
		*/
		/*
		struct MessageDeltaGameState
		{
			std::vector<SerializedEntity> Entities;
		};
		*/
	}
}