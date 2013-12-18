#pragma once
#include "Menu.h"
#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/Network/MessageHandler.h>
#include "GameStates.h"
namespace RootForce
{
	

	class Menustate
	{
	public:
		Menustate();
		void Initialize(RootEngine::GameSharedContext* p_engineContext, std::shared_ptr<RootForce::Network::MessageHandler> p_networkHandler);
		GameStates::GameStates Update();
		void Render();
		GameStates::PlayData GetPlayData() { return m_playData; }
		
	private:
		RootEngine::GameSharedContext* m_engineContext;
		std::shared_ptr<RootForce::Network::MessageHandler> m_networkHandler;
		std::shared_ptr<Menu> m_menu;
		GameStates::PlayData m_playData;
		
	};
}