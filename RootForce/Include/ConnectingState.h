#pragma once

#include <memory>
#include <RootForce/Include/GameStates.h>
#include <RootSystems/Include/Network/Client.h>
#include <RootSystems/Include/Network/Server.h>
#include <RootSystems/Include/Network/MessageHandlers.h>
#include <RootSystems/Include/Network/NetworkEntityMap.h>
#include <RootSystems/Include/PlayerSystem.h>

namespace RootForce
{
	/** Manages the state while the client is connecting to a server. During this time a player entity will not exist, hence several systems must not run. */
	class ConnectingState
	{
	public:
		ConnectingState(NetworkContext& p_networkContext, SharedSystems& p_sharedSystems);

		void Initialize();
		void Enter(const GameStates::PlayData& p_playData);
		void Exit();
		GameStates::GameStates Update();
	private:
		NetworkContext& m_networkContext;
		SharedSystems& m_sharedSystems;
	};
}