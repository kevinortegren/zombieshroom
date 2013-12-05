#pragma once

#include <Utility\ECS\Include\Component.h>

namespace RootForce
{
	struct PhysicsAccessor: public ECS::Component<PhysicsAccessor>
	{
		int* m_handle;
	};
}