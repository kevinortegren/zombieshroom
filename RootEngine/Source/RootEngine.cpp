#include <RootEngine.h>
#include <iostream>

namespace RootEngine
{
	typedef Network::NetworkManager* (*GETNETWORKINTERFACE)();

    Context::Context()
    {
		// Load the network module
		// TODO: Perhaps abstract the DLL loading code
		{
			HMODULE library = LoadLibraryA("Network.dll");
			if (library != nullptr)
			{
				GETNETWORKINTERFACE libGetNetworkInterface = (GETNETWORKINTERFACE) GetProcAddress(library, "GetNetworkInterface");
				if (libGetNetworkInterface != nullptr)
				{
					m_networkInterface = (Network::NetworkManager*)libGetNetworkInterface();
				}
				else
				{
					// TODO: Log error
				}
			}
			else
			{
				// TODO: Log error
			}
		}


		// TODO: Load the rest of the submodules
    }
}