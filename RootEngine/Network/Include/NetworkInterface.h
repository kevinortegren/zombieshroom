#pragma once
#include "Networker.h"
#include <RootEngine/Include/SubsystemSharedContext.h>

#if defined(_WINDLL)
#define NETSYS_DLL_EXPORT __declspec(dllexport)
#else
#define NETSYS_DLL_EXPORT __declspec(dllimport)
#endif

// Peer type is used to specify whether to initialize the networking instance as a client or server
namespace PeerType
{
	enum PeerType
	{
		CLIENT,
		SERVER
	};
}

namespace Network
{
	class NetworkInterface : public RootEngine::SubsystemInterface
	{
	public:
		// Initializes the network socket as either server or client
		virtual void Initialize(PeerType::PeerType p_peerType) = 0;
		// Returns the network interface
		virtual Networker* GetNetworkSystem() = 0;
	};
}

extern "C" 
{
	typedef Network::NetworkInterface* (*GETNETWORKINTERFACE) (RootEngine::SubsystemSharedContext);
	NETSYS_DLL_EXPORT Network::NetworkInterface* GetNetworkInterface(RootEngine::SubsystemSharedContext p_context);
}
