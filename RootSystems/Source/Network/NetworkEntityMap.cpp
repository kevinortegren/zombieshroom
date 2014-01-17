#include <Network/NetworkEntityMap.h>
#include <RakNet/BitStream.h>

namespace RootForce
{
	namespace Network
	{
		SequenceIDMap NetworkComponent::s_sequenceIDMap;

		void SerializeEntity(RakNet::BitStream* p_bs, ECS::Entity* p_entity, const std::string& p_entityScript, const NetworkEntityMap& p_map, ECS::World* p_world)
		{
			/*
			// Serialize the type
			{
				auto it = std::find(p_map.begin(), p_map.end(), p_entity);
				p_bs->Serialize(true, it->first.SynchronizedID);
			}

			// Serialize the script name
			p_bs->Serialize(true, RakNet::RakString(p_entityScript.c_str()));

			// Serialize all components
			std::vector<std::pair<unsigned int, ECS::ComponentInterface*>> components = p_world->GetEntityManager()->GetAllComponents(p_entity);
			for (auto it = components.begin(); it != components.end(); it++)
			{
				
				switch (it->first)
				{
					case ComponentType::TRANSFORM:
					{
						p_bs->Serialize(true, it->first);

						Transform* c = (Transform*) it->second;

						for (int i = 0; i < 3; ++i)
							p_bs->Serialize(true, c->m_position[i]);
						for (int i = 0; i < 4; ++i)
							p_bs->Serialize(true, c->m_orientation.GetQuaternion()[i]);
						for (int i = 0; i < 3; ++i)
							p_bs->Serialize(true, c->m_scale[i]);
					} break;

					case ComponentType::POINTLIGHT:
					{
						p_bs->Serialize(true, it->first);

						PointLight* c = (PointLight*) it->second;

						for (int i = 0; i < 4; ++i)
							p_bs->Serialize(true, c->m_color[i]);
						for (int i = 0; i < 3; ++i)
							p_bs->Serialize(true, c->m_attenuation[i]);
						p_bs->Serialize(true, c->m_range);
					} break;

					case ComponentType::HEALTH:
					{
						p_bs->Serialize(true, it->first);
					} break;


				}
			}
			*/
		}

		ECS::Entity* DeserializeEntity(RakNet::BitStream* p_bs, const NetworkEntityMap& p_map, ECS::World* p_world)
		{
			/*
			NetworkEntityID id;
			p_bs->Serialize(false, id);

			RakNet::RakString scriptName;
			p_bs->Serialize(false, scriptName);

			while (p_bs->GetNumberOfUnreadBits() > 0)
			{
				unsigned int type;
				p_bs->Serialize(false, type);

				switch (type)
				{
					case ComponentType::TRANSFORM:
					{
					} break;
				}
			}

			return nullptr;
			*/
			return nullptr;
		}
	}
}