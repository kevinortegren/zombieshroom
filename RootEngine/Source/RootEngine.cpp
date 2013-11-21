#include <RootEngine.h>
#include <iostream>

#include <glm/glm.hpp>

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

		// Load engine modules.
		m_world = new ECS::World;
    }

	Context::~Context()
	{
		delete m_world;
	}

	ECS::World* Context::GetWorld()
	{ 
		return m_world; 
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