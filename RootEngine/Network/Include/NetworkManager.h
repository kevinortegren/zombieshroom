#pragma once

#include "Networker.h"
#include <RootEngine/Include/SubsystemSharedContext.h>

#if defined(_WINDLL)
#define NETSYS_DLL_EXPORT __declspec(dllexport)
#else
#define NETSYS_DLL_EXPORT __declspec(dllimport)
#endif

namespace RootEngine
{
	namespace Network
	{
		// Peer type is used to specify whether to initialize the networking instance as a client or server
		namespace PeerType
		{
			enum PeerType
			{
				CLIENT,
				SERVER
			};
		}


		class NetworkInterface : public RootEngine::SubsystemInterface
		{
		public:
			// Initializes the network socket as either server or client
			virtual void Initialize(PeerType::PeerType p_peerType) = 0;
			// Returns the network interface
			virtual Networker* GetNetworkSystem() = 0;
		};

		class NetworkManager : public NetworkInterface
		{
		public:
			void Startup();
			void Shutdown();

			void Initialize(PeerType::PeerType p_peerType);
			Networker* GetNetworkSystem() { return m_networkSys; }
			static NetworkManager* GetInstance();

		private:
			Networker* m_networkSys;
			static NetworkManager* s_networkManager;

		};
	}
}

extern "C" 
{
	typedef RootEngine::Network::NetworkInterface* (*GETNETWORKINTERFACE) (RootEngine::SubsystemSharedContext);
	NETSYS_DLL_EXPORT RootEngine::Network::NetworkInterface* CreateNetwork(RootEngine::SubsystemSharedContext p_context);
}
