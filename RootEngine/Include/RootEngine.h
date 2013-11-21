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

	class ContextInterface
	{
	public:
		virtual ECS::World* GetWorld() = 0;
	};

	class Context : public ContextInterface
    {
    public:
        Context(int flags);
		~Context();

		ECS::World* GetWorld() { return m_world; }

	private:

		void LoadNetwork();

		/** Add interface classes */		
		NetworkManager* m_networkInterface;
		ECS::World* m_world;

    };

	extern "C"
	{
		ROOTENGINE_DLL_EXPORT ContextInterface* CreateContext(int flags);
	}
}