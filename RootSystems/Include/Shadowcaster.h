#pragma once

#include <Utility/ECS/Include/Component.h>
#include <RootSystems/Include/Frustum.h>

namespace RootForce
{
	struct Shadowcaster : public ECS::Component<Shadowcaster>
	{
		Shadowcaster() : m_directionalLightSlot(0) {}

		int m_directionalLightSlot;
	};
}