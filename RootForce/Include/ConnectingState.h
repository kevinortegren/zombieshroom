#pragma once

#include <RootForce/Include/GameStates.h>
#include <RootSystems/Include/Network/Client.h>
#include <RootSystems/Include/Network/Server.h>
#include <RootSystems/Include/Network/MessageHandlers.h>

namespace RootForce
{
	class ConnectingState
	{
	public:
		ConnectingState();

		void Initialize(const GameStates::PlayData& playData);
		GameStates::GameStates Update();
	private:
		RootForce::Network::Client* m_client;
		RootForce::Network::Server* m_server;
	};
}