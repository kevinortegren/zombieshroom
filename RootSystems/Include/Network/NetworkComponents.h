#pragma once

#include <Utility/ECS/Include/Component.h>

namespace RootForce
{
	namespace Network
	{
		struct NetworkClientComponent : public ECS::Component<NetworkClientComponent>
		{
			enum ClientState
			{
				EXPECTING_USER_INFO,
				CONNECTED
			};

			uint8_t UserID;
			ClientState State;
			std::string Name;
		};

		/*
		struct NetworkComponent : public ECS::Component<NetworkComponent>
		{

		};
		*/
	}
}