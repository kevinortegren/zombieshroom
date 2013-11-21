#include <RootEngine.h>
#include <RootEngine/Render/Include/RenderInterface.h>
#include <iostream>

namespace RootEngine
{
	typedef NetworkManager* (*GETNETWORKINTERFACE)();
	typedef Renderer::RendererInterface* (*CREATERENDERER)();

	Context::Context(int flag)
	{
		std::cout << "Creating Engine Context" << std::endl;

		if((flag & SubsystemInit::INIT_NETWORK) == SubsystemInit::INIT_NETWORK)
		{
			LoadNetwork();
		}

		if((flag & SubsystemInit::INIT_RENDER) == SubsystemInit::INIT_RENDER)
		{
			LoadRender();
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

	void Context::LoadRender()
	{
		// Load the render module
		// TODO: Perhaps abstract the DLL loading code

		HMODULE library = LoadLibraryA("Render.dll");
		if (library != nullptr)
		{
			CREATERENDERER libGetRenderer = (CREATERENDERER) GetProcAddress(library, "CreateRenderer");
			if (libGetRenderer != nullptr)
			{
				m_renderer = (Renderer::RendererInterface*)libGetRenderer();
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

	Renderer::RendererInterface* Context::GetRenderer()
	{
		return m_renderer;
	}
	
	ContextInterface* CreateContext(int flags)
	{
		return new Context(flags);
	}

}