#include <Network/NetworkEntityMap.h>
#include <RakNet/BitStream.h>

namespace RootForce
{
	namespace Network
	{
		void Serialize(bool p_write, RakNet::BitStream* p_bs, ECS::ComponentInterface* p_component, ComponentType::ComponentType p_type)
		{
			
		}

		void Serialize(bool p_write, RakNet::BitStream* p_bs, ECS::Entity* p_entity, NetworkEntityID p_id)
		{

		}
	}
}