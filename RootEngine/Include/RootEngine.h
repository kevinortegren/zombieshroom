#pragma once

#if defined(_WINDLL)
	#define ROOTENGINE_DLL_EXPORT __declspec(dllexport)
#else
	#define ROOTENGINE_DLL_EXPORT __declspec(dllimport)
#endif

#include <RootEngine/Network/Include/NetworkManager.h>
#include <RootEngine/Include/ECS/World.h>

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

    class Context
    {
    public:
        Context(int flags);
		~Context();

	private:

		void LoadNetwork();

		/** Add interface classes */		
		NetworkManager* m_networkInterface;
		ECS::World* m_world;

    };

	ROOTENGINE_DLL_EXPORT Context* CreateContext(int flags);
}