#include <RootEngine.h>
#include <iostream>

namespace RootEngine
{
	typedef NetworkManager* (*GETNETWORKINTERFACE)();

    Context::Context(int flag)
    {
		std::cout << "Creating Engine Context" << std::endl;

		if((flag & SubsystemInit::INIT_NETWORK) == SubsystemInit::INIT_NETWORK)
		{
			LoadNetwork();
		}

		// TODO: Load the rest of the submodules
		
		m_world = new ECS::World;
    }

	Context::~Context()
	{
		if(m_world != nullptr)
		{
			delete m_world;
			m_world = nullptr;
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
	
	Context* CreateContext(int flags)
	{
		return new Context(flags);
	}
}