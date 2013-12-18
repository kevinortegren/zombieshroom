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
		Menustate();
		void Initialize(RootEngine::GameSharedContext* p_engineContext, RootForce::Network::Client* p_client, RootForce::Network::ClientMessageHandler* p_clientMessageHandler);
		GameStates::GameStates Update();
		void Render();
		GameStates::PlayData GetPlayData() { return m_playData; }
		
	private:
		RootEngine::GameSharedContext* m_engineContext;

		RootForce::Network::Client* m_client;
		std::shared_ptr<RootSystems::LanList> m_lanList;

		std::shared_ptr<Menu> m_menu;
		GameStates::PlayData m_playData;
		
	};
}