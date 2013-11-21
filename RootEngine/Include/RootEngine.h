#pragma once

#if defined(_WINDLL)
	#define ROOTENGINE_DLL_EXPORT __declspec(dllexport)
#else
	#define ROOTENGINE_DLL_EXPORT __declspec(dllimport)
#endif

#include <RootEngine/Network/Include/NetworkManager.h>

namespace RootEngine
{
    class ROOTENGINE_DLL_EXPORT Context
    {
    public:
        Context();

		/** Add interface classes */
		NetworkManager* m_networkInterface;
    };
}