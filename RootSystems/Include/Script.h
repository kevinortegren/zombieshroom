#pragma once

#include <Utility/ECS/Include/Component.h>

namespace RootForce
{
    struct Script : public ECS::Component<Script>
	{
		std::string Name;
	};
}