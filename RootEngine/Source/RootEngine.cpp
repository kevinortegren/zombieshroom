#include <RootEngine.h>
#include <iostream>

namespace RootEngine
{
	typedef NetworkManager* (*GETNETWORKINTERFACE)();

    Context::Context(int flag)
    {
		std::cout << "Creating Engine Context" << std::endl;

		// Load external dlls.
		if((flag & SubsystemInit::INIT_NETWORK) == SubsystemInit::INIT_NETWORK)
		{
			LoadNetwork();
		}
    }

	void Context::LoadNetwork()
	{
		// Load the network module
		// TODO: Perhaps abstract the DLL loading code
		
		HMODULE library = LoadLibraryA("Network.dll");
		if (library != nullptr)
		{
			GETNETWORKINTERFACE libGetNetworkInterface = (GETNETWORKINTERFACE) GetProcAddress(library, "GetNetworkInterface");
			if (libGetNetworkInterface != nullptr)
			{
				m_networkInterface = (NetworkManager*)libGetNetworkInterface();
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
	
	ContextInterface* CreateContext(int flags)
	{
		static Context context(flags);
		return &context;
	}
}