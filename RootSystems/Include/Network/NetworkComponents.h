#pragma once

#include <Utility/ECS/Include/Component.h>

namespace RootForce
{
	namespace Network
	{
		struct NetworkClientComponent : public ECS::Component<NetworkClientComponent>
		{
			uint8_t UserID;
		};

		struct NetworkComponent : public ECS::Component<NetworkComponent>
		{

		};
	}
}