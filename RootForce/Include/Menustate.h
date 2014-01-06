#pragma once
#include <RootForce/Include/Menu.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/Network/Client.h>
#include <RootSystems/Include/Network/LanList.h>
#include <GameStates.h>

namespace RootForce
{
	/** Handles client messages in the menu state. Will only care for ping/pong messages. */
	class ClientMenuMessageHandler : public RootForce::Network::MessageHandler
	{
	public:
		ClientMenuMessageHandler(RakNet::RakPeerInterface* p_peer, Logging* p_logger, RootSystems::LanList* p_lanList);

		bool ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet);
	private:
		RootSystems::LanList* m_lanList;
	};


	/** Handles the game menu */
	class MenuState
	{
	public:
		MenuState(std::shared_ptr<RootForce::Network::Client>& p_client, std::shared_ptr<RootForce::Network::Server>& p_server);

		void Initialize(const std::string& p_workingDir);
		void Enter();
		void Exit();
		GameStates::GameStates Update();
	private:
		std::shared_ptr<RootForce::ClientMenuMessageHandler> m_menuMessageHandler;
		std::shared_ptr<RootForce::Network::Client>& m_client;
		std::shared_ptr<RootForce::Network::Server>& m_server;
		std::shared_ptr<RootSystems::LanList> m_lanList;

		std::shared_ptr<Menu> m_menu;

		std::string m_workingDir;
	};
}