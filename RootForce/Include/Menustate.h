#pragma once
#include "Menu.h"
#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/Network/Client.h>
#include <RootSystems/Include/Network/LanList.h>
#include <GameStates.h>

namespace RootForce
{
	class Menustate
	{
	public:
		void Initialize(RootForce::Network::Client* p_client);
		void Enter();
		void Exit();
		GameStates::GameStates Update();

		GameStates::PlayData GetPlayData() { return m_playData; }
	private:
		RootForce::Network::Client* m_client;
		std::shared_ptr<RootForce::Network::ClientMessageHandler> m_clientMessageHandler;
		std::shared_ptr<RootSystems::LanList> m_lanList;

		std::shared_ptr<Menu> m_menu;
		GameStates::PlayData m_playData;
	};
}