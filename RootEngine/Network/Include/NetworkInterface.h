#pragma once
#include "Networker.h"

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

class NetworkInterface abstract
{
public:
	// Initializes the network socket as either server or client
	virtual void Initialize(PeerType::PeerType p_peerType) = 0;
	// Returns the network interface
	virtual Networker* GetNetworkSystem() = 0;
};

extern "C++" NETSYS_DLL_EXPORT NetworkInterface* APIENTRY GetNetworkInterface();
