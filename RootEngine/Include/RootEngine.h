#pragma once

#if defined(_WINDLL)
	#define ROOTENGINE_DLL_EXPORT __declspec(dllexport)
#else
	#define ROOTENGINE_DLL_EXPORT __declspec(dllimport)
#endif

#include <RootEngine/Network/Include/NetworkManager.h>
#include <RootEngine/Render/Include/Renderer.h>


namespace RootEngine
{
	namespace SubsystemInit
	{
		enum SubsystemInit
		{
			INIT_NETWORK = 1,
			INIT_INPUT = 2,
			INIT_RENDER = 4,
			INIT_ALL = INIT_NETWORK | INIT_INPUT | INIT_RENDER
		};
	}


	class ContextInterface
	{
	public:
		virtual Render::RendererInterface* GetRenderer() = 0;
	};


	class Context : public ContextInterface
	{
	public:

		Context(int flags);
		~Context();

		Render::RendererInterface* GetRenderer();

	private:
		void LoadNetwork();
		void LoadRender();

		Network::NetworkManager* m_networkInterface;
		Render::RendererInterface* m_renderer;
	};

	extern "C"
	{
		ROOTENGINE_DLL_EXPORT ContextInterface* CreateContext(int flags);
	}
}