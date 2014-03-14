#pragma once
#include <Utility/ECS/Include/World.h>
#include <RakNet/BitStream.h>
#include <vector>

namespace RootForce
{
	class DeserializationSystem : public ECS::VoidSystem
	{
	public:
		DeserializationSystem(ECS::World* p_world);
		void SetData(const RakNet::BitStream* p_bs);

		void Process();

	private:
		std::vector<unsigned char> m_data;
	};
}