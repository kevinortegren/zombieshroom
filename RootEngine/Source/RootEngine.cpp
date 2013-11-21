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

		// Load external dlls.
		if((flag & SubsystemInit::INIT_NETWORK) == SubsystemInit::INIT_NETWORK)
		{
			LoadNetwork();
		}
		if((flag & SubsystemInit::INIT_RENDER) == SubsystemInit::INIT_RENDER)
		{
			LoadRender();
		}

		// TODO: Load the rest of the submodules
		
	}

	Context::~Context()
	{

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
		static Context context(flags);
		return &context;
	}

}